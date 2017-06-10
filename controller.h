#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QVariantList>
#include <memory>
#include "constants.h"
#include "node.h"

using std::make_shared;
using std::shared_ptr;
using std::unique_ptr;

class CalculaterNode;

class Controller : public QObject
{
    Q_OBJECT

signals:
    void submitClick(int item);
    void newState(QList<int> newState);

public slots :
    void clicked(int pos);
    void init(bool redTurn, int itemCount);
    void setTakeOverAndPlay(bool takeOver);
    void cameBackWithResult(int res, int action);

public:
    explicit Controller();

    static int itemCount;
    static int itemsInARow;
    bool takeOver;

    explicit Controller(const Controller& rhs) = delete;
    Controller& operator= (const Controller& rhs) = delete;
    virtual ~Controller();

private:
    //    shared_ptr<Colors> colorGame;
    shared_ptr<Node> currNode;

    void publishNewState(int pos);
    QList<CalculaterNode*> calculationThreads;
    int cameBack;
//    int toComeBack;
    int actionToTake;
    int lastRes;
    int currDepthLimit;
    void play();
    shared_ptr<QList<shared_ptr<Pos>>> clickables;

};

#endif // CONTROLLER_H
