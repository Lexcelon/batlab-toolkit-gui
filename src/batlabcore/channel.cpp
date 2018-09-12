#include "channel.h"

channel::channel(QObject *parent) : QObject(parent)
{
    QState* s_unknown = new QState();
    QState* s_no_cell = new QState();
    QState* s_backwards = new QState();
    QState* s_idle = new QState();
    QState* s_stopped = new QState();

    QState* s_current_flowing = new QState();
    QState* s_charge = new QState(s_current_flowing);
    QState* s_discharge = new QState(s_current_flowing);
    QState* s_impedance = new QState(s_current_flowing);
    s_current_flowing->setInitialState(s_charge);

    channelStateMachine.addState(s_unknown);
    channelStateMachine.addState(s_no_cell);
    channelStateMachine.addState(s_backwards);
    channelStateMachine.addState(s_idle);
    channelStateMachine.addState(s_stopped);
    channelStateMachine.addState(s_current_flowing);

    channelStateMachine.setInitialState(s_unknown);
    channelStateMachine.start();
}
