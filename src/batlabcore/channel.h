#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QStateMachine>

class channel : public QObject
{
    Q_OBJECT
public:
    explicit channel(QObject *parent = nullptr);

signals:

public slots:

private:
    QStateMachine channelStateMachine;
    QState* s_unknown;
    QState* s_no_cell;
    QState* s_backwards;
    QState* s_idle;
    QState* s_stopped;

    QState* s_current_flowing;
    QState* s_charge;
    QState* s_discharge;
    QState* s_impedance;
};

#endif // CHANNEL_H
