#ifndef CALCULATERNODE_H
#define CALCULATERNODE_H

#include <QObject>
#include "constants.h"
#include <QThread>
#include <memory>
#include "node.h"

using std::make_shared;
using std::unique_ptr;

class Node;

class CalculaterNode : public QObject
{
    Q_OBJECT
private:
    shared_ptr<Node> input;
    int action;
    QThread* thread;
    int depthLimit;

    int getValue(shared_ptr<Node> node, int alpha, int beta, bool isMaxPlayer);
    int minValue(shared_ptr<Node> node);
    int maxValue(shared_ptr<Node> node);
//    static int heuristic(shared_ptr<Node> node);

signals:
    void resultFound(int result, int action);

public slots:
    void calculate();
    void finished();

public:
    explicit CalculaterNode(shared_ptr<Node> input, int action, int depthLimit);
//    static void click(shared_ptr<Node> node, int pos);

    explicit CalculaterNode(const CalculaterNode& rhs) = delete;
    CalculaterNode& operator= (const CalculaterNode& rhs) = delete;
    virtual ~CalculaterNode();
    QThread *getThread() const;
};

#endif // CALCULATERNODE_H
