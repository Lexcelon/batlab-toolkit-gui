#ifndef CHANNEL_H
#define CHANNEL_H

#include "ctime"

#include <QObject>
#include <QTimer>

#include "batlablib.h"
#include "cellplaylist.h"
#include "batlabpacket.h"
#include "encoder.h"

class Batlab;
class BatlabManager;

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(int slot, QObject *parent = nullptr);
    channelStatusInfo info;
    bool testInProgress() { return m_test_state != TS_IDLE; }
    ChannelMode mode() { return m_mode; }

signals:

public slots:
    void startTest();
    void abortTest();
    void handleSerialResponseBundleReady(batlabPacketBundle bundle);

private:
    QTimer *m_channelPeriodicCheckTimer;

    ChannelMode m_mode;
    TestState m_test_state;

    std::time_t m_ts;
    std::time_t m_start_time;
    std::time_t m_last_lvl2_time;
    std::time_t m_last_impedance_time;
    std::time_t m_rest_time;

    int m_voltage_count;
    float m_voltage_avg;
    float m_voltage_prev;
    int m_voltage_error_count;

    int m_current_count;
    float m_current_avg;
    float m_current_prev;
    quint16 m_current_setpoint;

    float m_z_avg;
    float m_z_count;

    float m_vcc;

    float m_e;
    float m_q;

    float m_temperature0;
    float m_delta_t;

    int m_current_cycle;

    std::time_t m_pulse_discharge_on_time;
    std::time_t m_pulse_discharge_off_time;
    std::time_t m_pulse_charge_on_time;
    std::time_t m_pulse_charge_off_time;
    bool m_pulse_state;

    bool m_trickle_engaged;

    void periodicCheck();
    void handlePeriodicCheckResponse(QVector<BatlabPacket> response);
    void handleStartTestResponse(QVector<BatlabPacket> response);
    void stateMachine();

    Batlab *batlab();
    CellPlaylist playlist();
};

#endif // CHANNEL_H
