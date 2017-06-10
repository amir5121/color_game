#ifndef CONSTANTS_H
#define CONSTANTS_H

#define DEPTH 3
#define DEBUGGING false
#define PROCESS_LIMIT 15
#define MULTI_THREADED true

template <class t>
class QList;

enum STATE {EMPTY, RED, GREEN};

typedef QList<QList<int>> t_state;


#endif // CONSTANTS_H
