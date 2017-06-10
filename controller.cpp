#include "controller.h"
#include <QDebug>
#include <math.h>
#include <calculaternode.h>
#include "constants.h"

using std::sqrt;

int Controller::itemCount;
int Controller::itemsInARow;

Controller::Controller() : cameBack(0), clickables(nullptr), takeOver(false), currDepthLimit(DEPTH)
{
}

void Controller::clicked(int pos)
{
    Node::click(currNode, pos);
    int redScore, greenScore;
    currNode->scoreSum(redScore, greenScore);
    qDebug() << "\nredScore" << redScore << "greenScore" << greenScore;
    if (takeOver || actionToTake != pos)
        play();
}

void Controller::setTakeOverAndPlay(bool takeOver)
{
    this->takeOver = takeOver;
    if(takeOver)
        play();
}


void Controller::init(bool redTurn, int itemCount)
{
    Controller::itemCount = itemCount;
    Controller::itemsInARow = sqrt(itemCount);
    QList<int> row;
    int currRow = 0;
    shared_ptr<t_state> startState = make_shared<t_state>();
    for (int i = 0; i < Controller::itemCount; i++) {
        if (currRow != i / Controller::itemsInARow) {
            startState->append(row);
            row.clear();
            currRow++;
        }
        row.append(EMPTY);
    }

    startState->append(row);

    (*startState)[0][0] = RED;
    (*startState)[0][Controller::itemsInARow - 1] = GREEN;
    (*startState)[Controller::itemsInARow - 1][0] = GREEN;
    (*startState)[Controller::itemsInARow - 1][Controller::itemsInARow - 1] = RED;


    currRow = 0;
    row.clear();
    shared_ptr<t_state> startScore = make_shared<t_state>();
    for (int i = 0; i < Controller::itemCount; i++) {
        if (currRow != i / Controller::itemsInARow) {
            startScore->append(row);
            row.clear();
            currRow++;
        }
        row.append(0);
    }


    startScore->append(row);

    (*startScore)[0][0] = 5;
    (*startScore)[0][Controller::itemsInARow - 1] = 5;
    (*startScore)[Controller::itemsInARow - 1][0] = 5;
    (*startScore)[Controller::itemsInARow - 1][Controller::itemsInARow - 1] = 5;

    currNode = make_shared<Node>(startState, startScore, 0, redTurn);

    if(DEBUGGING) currNode->heuristic();

}

void Controller::cameBackWithResult(int res, int action)
{

    if (cameBack == 0) {
        lastRes = res;
    }

    if (res >= lastRes) {
        actionToTake = action;
        lastRes = res;
    }


    if (!clickables->isEmpty()) {
        shared_ptr<Pos>  pos = clickables->takeFirst();
        CalculaterNode *calculator = new CalculaterNode(currNode, pos->x * Controller::itemsInARow + pos->y, (DEBUGGING ? 1 : currDepthLimit));
        connect(calculator, &CalculaterNode::resultFound, this, &Controller::cameBackWithResult);
        calculationThreads.append(calculator);
    }

    cameBack++;

    if (cameBack == calculationThreads.size()) {

        foreach (CalculaterNode* calcNode, calculationThreads) {
            calcNode->getThread()->quit();
            calcNode->getThread()->wait();
        }

        cameBack = 0;
        emit submitClick(actionToTake);
        calculationThreads.clear();
        qDebug() << "action:" << actionToTake;
    }
}


Controller::~Controller()
{

}

void Controller::play()
{
    if (clickables != nullptr) clickables->clear();


    //    foreach (QList<int> row, (*currNode->getState())) {
    //        qDebug() << row;
    //    }

    if (MULTI_THREADED) {
        clickables = currNode->getMoves();

        currDepthLimit = DEPTH;

        if (clickables->size() < 8) currDepthLimit += 2;
        else if (clickables->size() < 13) currDepthLimit++;
        else if (clickables->size() < 22);
        else if (clickables->size() > 28) currDepthLimit--;


        qDebug() << "clickables.size()" << clickables->size() << "depth" << currDepthLimit;


        for (int i = 0; i < PROCESS_LIMIT && i < clickables->size(); ++i) {
            shared_ptr<Pos>  pos = clickables->takeFirst();
            CalculaterNode *calculator = new CalculaterNode(currNode, pos->x * Controller::itemsInARow + pos->y, (DEBUGGING ? 1 : currDepthLimit));
            connect(calculator, &CalculaterNode::resultFound, this, &Controller::cameBackWithResult);
            calculationThreads.append(calculator);
        }

    } else {
        CalculaterNode *calculator = new CalculaterNode(currNode, 0, DEPTH);
        connect(calculator, &CalculaterNode::resultFound, this, &Controller::cameBackWithResult);
        calculationThreads.append(calculator);
    }
}
