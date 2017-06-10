#pragma once
#include <QObject>
#include <QList>
#include <memory>
#include "constants.h"

using std::shared_ptr;
using std::make_shared;

class Controller;

class Colors : public QObject
{
    Q_OBJECT
public:
    explicit Colors(Controller* controller, bool redTurn, int itemCount);

    void clicked(int pos);

    explicit Colors(const Colors& rhs) = delete;
    Colors& operator= (const Colors& rhs) = delete;
    virtual ~Colors();

signals:
    void submitClick(int itemNumber);

public slots :
    void cameBackWithResult(int result);
private:
    Controller* controller;
    bool redTurn;
    int itemCount;
    int itemsInARow;
    shared_ptr<t_state> currState;
    bool takeOver;

};
