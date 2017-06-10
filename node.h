#ifndef NODE_H
#define NODE_H
#include "constants.h"
#include <QList>

#include <memory>

using std::shared_ptr;

struct Pos{
    const uchar x;
    const uchar y;

    Pos(uchar x, uchar y):x(x), y(y){}
};

class Node
{
public:
    Node(shared_ptr<t_state> state, shared_ptr<t_state> score, unsigned char depth, bool redTurn);

    explicit Node(const Node& rhs) = delete;
    Node& operator= (const Node& rhs) = delete;
    virtual ~Node();

    unsigned char getDepth() const;
    shared_ptr<QList<shared_ptr<Node> > > expand();

    shared_ptr<t_state> getState() const;

    shared_ptr<t_state> getScore() const;

    static void click(shared_ptr<Node> node, int pos);

    bool getRedTurn() const;

    void setRedTurn(bool value);

    static int getValueCalls;

    void scoreSum(int &redScore, int &greenScore);

    bool isTerminal();

    int heuristic();

    shared_ptr<QList<shared_ptr<Pos> > > getMoves();

    void checkSurrounding(int i, int j, int& emptyCell) const;

private:
    shared_ptr<t_state> state;
    shared_ptr<t_state> score;
    unsigned char depth;
    bool redTurn;
    int action;
    shared_ptr<QList<shared_ptr<Pos>>> moves;

    static int diff;

    static long expanded;

//    int min_node()
};

#endif // NODE_H
