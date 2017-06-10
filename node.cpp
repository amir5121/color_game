#include "node.h"
#include "controller.h"
#include <QDebug>
#include "calculaternode.h"

long Node::expanded;
int Node::getValueCalls;
int Node::diff;

Node::Node(shared_ptr<t_state> state, shared_ptr<t_state> score, unsigned char depth, bool redTurn) :
    state(state), score(score), depth(depth), redTurn(redTurn), moves(nullptr)
{
    Node::diff = 5;
}

unsigned char Node::getDepth() const
{
    return depth;
}

shared_ptr<QList<shared_ptr<Node>>> Node::expand()
{
    shared_ptr<QList<shared_ptr<Pos>>> clickables = getMoves();

    shared_ptr<QList<shared_ptr<Node>>> childs = make_shared<QList<shared_ptr<Node>>>();

    for (int i = 0; i < clickables->size(); i++) {
        shared_ptr<t_state> childState = make_shared<t_state>();
        (*childState) = (*state);

        shared_ptr<t_state> childScore = make_shared<t_state>();
        (*childScore) = (*score);

        shared_ptr<Node> child = make_shared<Node> (childState, childScore, depth + 1,
                                                    redTurn //this will be updated in the click method call down here
                                                    );

        Node::click(child, (*clickables)[i]->x * Controller::itemsInARow + (*clickables)[i]->y);

        childs->append(child);

    }

    return childs;

}

void Node::click(shared_ptr<Node> node, int pos)
{
    int x = pos / Controller::itemsInARow, y = pos % Controller::itemsInARow, currX = -1, currY = -1;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            currX = x + i;
            currY = y + j;
            if (currX >= 0 && currX < Controller::itemsInARow && currY >= 0 && currY < Controller::itemsInARow) {
                if ((*node->getState())[currX][currY] != EMPTY) {
                    (*node->getState())[currX][currY] = node->getRedTurn() ? RED : GREEN;
                    (*node->getScore())[currX][currY]++;
                }
            }
        }
    }


    (*node->getState())[x][y] = node->getRedTurn() ? RED : GREEN;
    (*node->getScore())[x][y]++;

    //    qDebug() << "pos:" << pos;
    //    foreach (QList<int> row, ((*node->getScore()))) {
    //        qDebug() << row;
    //    }
    //    qDebug() <<"";
    //    foreach (QList<int> row, ((*node->getState()))) {
    //        qDebug() << row;
    //    }


    node->setRedTurn(!node->getRedTurn());
    //    redTurn = !redTurn;

    //    if (Controller::takeOver || node->getRedTurn()) {

    //    }
}


shared_ptr<t_state> Node::getState() const
{
    return state;
}

shared_ptr<t_state> Node::getScore() const
{
    return score;
}

bool Node::getRedTurn() const
{
    return redTurn;
}

void Node::setRedTurn(bool value)
{
    redTurn = value;
}

void Node::scoreSum(int& redScore, int& greenScore)
{
    redScore = 0;
    greenScore = 0;
    for (int i = 0; i < Controller::itemsInARow; ++i) {
        for (int j = 0; j < Controller::itemsInARow; ++j) {
            switch ((*state)[i][j]) {
            case RED:
                redScore += (*score)[i][j];
                break;
            case GREEN:
                greenScore += (*score)[i][j];
                break;
            default:
                break;
            }
        }
    }
}

bool Node::isTerminal()
{
    foreach (QList<int> row, (*state)) {
        foreach (int i, row) {
            if (i == EMPTY) return false;
        }
    }
    return true;
}

int Node::heuristic()
{

    bool isRedPlaying = ((depth % 2 == 0) ? !redTurn : redTurn);

    int myScore = 0, rivalScore = 0;

    //    bool print = DEBUGGING;

    float heuristic = 5000, stableScore = 0;

    int i, j, emptyCell = 0/*, unreachableCells = 0*/;
    float emptyCells = Controller::itemsInARow * Controller::itemsInARow + 2;


    for (i = 0; i < Controller::itemsInARow; i++) {
        for (j = 0; j < Controller::itemsInARow; j++) {
            if ((*state)[i][j] != EMPTY) {
                checkSurrounding(i, j, emptyCell/*, unreachableCells*/);

                if (emptyCell > 0) {
                    if (emptyCell % 2 == 0) {
                        heuristic += /*unreachableCells / 2.5 +*/ (*score)[i][j] / emptyCell;
                    } else {
                        heuristic -= /*unreachableCells / 2.5 +*/ (*score)[i][j] / emptyCell;
                    }
                }

                if ((*state)[i][j] == (isRedPlaying ? RED : GREEN)) {

                    if (emptyCell == 0) {
                        stableScore += (*score)[i][j]; //our stable score MATTERS!
                    }

                    myScore += (*score)[i][j];
                } else {
                    rivalScore += (*score)[i][j];
                }

                emptyCells--;
            }
        }
    }

    //    if (print) qDebug() << "-----heuristic" << heuristic << "myScore" << myScore << "rivalScore" << rivalScore << "emptyCells" << emptyCells;

    heuristic +=
            //            (myScore - rivalScore) / 7. + /* blind look at the score giving a little damn*/
            ((myScore - rivalScore) / 3.5) / (emptyCells) +  /*this matter more and more as we progress towards the end of the game*/
            stableScore / 2.5;

    //    if (print) qDebug() << "heuristic" << heuristic << "PLAYER" << (isRedPlaying ? RED : GREEN) << "myScore" << myScore << "rivalScore" << rivalScore;

    //    if (print) {
    //        foreach (QList<int> row, (*state)) {
    //            qDebug() << row;
    //        }

    //        qDebug() << "";

    //        foreach (QList<int> row, (*score)) {
    //            qDebug() << row;
    //        }

    //    }

    return heuristic;
}

shared_ptr<QList<shared_ptr<Pos> > > Node::getMoves()
{
    //    if (this->moves == nullptr) {
    bool added = false;
    int i, j, k, l;

    shared_ptr<QList<shared_ptr<Pos>>> moves = make_shared<QList<shared_ptr<Pos>>>();

    for (i = 0; i < Controller::itemsInARow; i++) {
        for (j = 0; j < Controller::itemsInARow; j++) {
            if ((*state)[i][j] == EMPTY) {
                for(k = -1; k <= 1; k++) {
                    for (l = -1; l <= 1; l++) {
                        if (i + k >= 0 && i + k < Controller::itemsInARow && j + l >= 0 && j + l < Controller::itemsInARow && (*state)[i + k][j + l] != EMPTY) {
                            moves->append(make_shared<Pos>(i, j));
                            added = true;
                            break;
                        }
                    }
                    if (added) break;
                }
            }
            added = false;
        }
    }

    this->moves = moves;
    //    }

    return moves;
}

void Node::checkSurrounding(int i, int j, int& emptyCell) const
{
    emptyCell = 0;
//    unreachableCells = 0;
    int k, l, currX, currY;
    for(k = -1; k <= 1; k++) {
        for (l = -1; l <= 1; l++) {
            currX = i + k;
            currY = j + l;
            if (currX >= 0 && currX < Controller::itemsInARow && currY >= 0 && currY < Controller::itemsInARow) {
                if((*state)[currX][currY] == EMPTY) {
                    if (k == 0 && l == 0) emptyCell--;
                    emptyCell++;
                }
            }
//            else {
//                unreachableCells++;
//            }
        }
    }
}



Node::~Node() {

}
