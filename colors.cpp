#include "colors.h"
#include <math.h>
#include <QDebug>
#include "controller.h"
#include "calculaternode.h"
#include <QThread>

using std::sqrt;

Colors::Colors(Controller *controller, bool redTurn, int itemCount) :
    controller(controller),
    redTurn(redTurn),
    itemCount(itemCount),
    itemsInARow(sqrt(itemCount)),
    takeOver(false)
{
    connect(this, &Colors::submitClick, controller, &Controller::submitClick);
    QList<int> row;
    int currRow = 0;
    currState = make_shared<t_state>();
    for (int i = 0; i < itemCount; i++) {
        if (currRow != i / itemsInARow) {
            currState->append(row);
            row.clear();
            currRow++;
        }
        row.append(EMPTY);
    }

    currState->append(row);

    (*currState)[0][0] = RED;
    (*currState)[0][itemsInARow - 1] = GREEN;
    (*currState)[itemsInARow - 1][0] = GREEN;
    (*currState)[itemsInARow - 1][itemsInARow - 1] = RED;

   CalculaterNode *calculator = new CalculaterNode((*currState), 23);
   connect(calculator, &CalculaterNode::resultFound, this, &Colors::cameBackWithResult);

}

void Colors::clicked(int pos)
{
    int currIndex = -1;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            currIndex = pos + (i * itemsInARow) + j;
            if (currIndex > 0 && currIndex < itemCount && (*currState)[currIndex / itemsInARow][currIndex % itemsInARow] != EMPTY) {
                (*currState)[currIndex / itemsInARow][currIndex % itemsInARow] = redTurn ? RED : GREEN;
            }
        }
    }

    (*currState)[pos / itemsInARow][pos % itemsInARow] = redTurn ? RED : GREEN;

    redTurn = !redTurn;

    if (takeOver || redTurn) {

    }
}

Colors::~Colors()
{

}

void Colors::cameBackWithResult(int result)
{
    qDebug() << result;
}

