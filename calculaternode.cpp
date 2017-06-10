#include "calculaternode.h"
#include <QDebug>

#include "controller.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

CalculaterNode::CalculaterNode(shared_ptr<Node> input, int action, int depthLimit):
    action(action),
    thread(new QThread()),
    depthLimit(depthLimit)
{
    shared_ptr<t_state> newState = make_shared<t_state>();
    *newState = (*input->getState());

    shared_ptr<t_state> newScore = make_shared<t_state>();
    *newScore = (*input->getScore());

    this->input = make_shared<Node>(newState, newScore, input->getDepth(), input->getRedTurn());
    if (MULTI_THREADED) Node::click(this->input, action);
    moveToThread(thread);
    connect(thread, &QThread::started, this, &CalculaterNode::calculate);
    connect(thread, &QThread::finished, this, &CalculaterNode::finished);
    thread->start();

}


void CalculaterNode::calculate()
{
    int res = getValue(input, INT_MIN, INT_MAX, false);
    //    int res = minValue(input);
    //    qDebug() << "calc res" << res << "action" << action << "redTurn" << input->getRedTurn();
    emit resultFound(res, action);

    //    thread->quit();
}

QThread *CalculaterNode::getThread() const
{
    return thread;
}

int CalculaterNode::getValue(shared_ptr<Node> node, int alpha, int beta, bool isMaxPlayer) {
    //    int depth = node->getDepth();
    //    Node::getValueCalls++;
    //    if (Node::getValueCalls % 1000 == 0) qDebug() << "getValueCalls" << Node::getValueCalls;
    if (node->getDepth() == depthLimit || node->isTerminal()) {
        return node->heuristic();
    } else {
        if (isMaxPlayer) {
            int v = INT_MIN;
            shared_ptr<QList<shared_ptr<Node>>> childs = node->expand();
            int size = childs->size();
//                    qDebug() << "calculating " << size ;
            for (int i = 0; i < size; i++) {
                v = MAX(v, getValue((*childs)[i], alpha, beta, false));

                alpha = MAX(alpha, v);
                if (beta < alpha) {
                    //                    qDebug() << "getValueCalls";
                    break;
                }
            }

            return v;
        } else {
            int v = INT_MAX;
            shared_ptr<QList<shared_ptr<Node>>> childs = node->expand();
            int size = childs->size();
            //        qDebug() << "calculating " << size ;
            for (int i = 0; i < size; i++) {
                v = MIN(v, getValue((*childs)[i], alpha, beta, true));
                beta = MIN(beta, v);
                if (beta < alpha) {
                    //                    qDebug() << "getValueCalls";
                    break;
                }
            }

            return v;
        }
    }
}

int CalculaterNode::minValue(shared_ptr<Node> node)
{
    //    int depth = node->getDepth();
    if (node->getDepth() == depthLimit) {
        return node->heuristic();
    } else {
        int v = INT_MAX;
        //        qDebug() << "minValue called " << node->getDepth();
        shared_ptr<QList<shared_ptr<Node>>> childs = node->expand();
        int size = childs->size();
        //        qDebug() << "calculating " << size ;
        for (int i = 0; i < size; i++) {
            v = MIN(v, maxValue((*childs)[i]));
        }

        return v;
    }
}

int CalculaterNode::maxValue(shared_ptr<Node> node)
{
    //    int depth = node->getDepth();
    if (node->getDepth() == depthLimit) {
        return node->heuristic();
    } else {
        int v = INT_MIN;
        //        qDebug() << "minValue called " << node->getDepth();
        shared_ptr<QList<shared_ptr<Node>>> childs = node->expand();
        int size = childs->size();
        //        qDebug() << "calculating " << size ;
        for (int i = 0; i < size; i++) {
            v = MAX(v, minValue((*childs)[i]));
        }

        return v;
    }
}


void CalculaterNode::finished()
{
    //    qDebug() << "finished";
    delete this;
    //    delete thread;
}

CalculaterNode::~CalculaterNode()
{
    //        delete thread;
}
