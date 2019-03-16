#ifndef CHANNEL_H
#define CHANNEL_H

#include "ctime"

#include <QObject>
#include <QTimer>

#include "batlablib.h"
#include "cellplaylist.h"
#include "batlabpacket.h"

class Batlab;

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(Batlab *batlab, int slot, QObject *parent = nullptr);
    channelStatusInfo info;
    bool testInProgress() { return m_test_state != TS_IDLE; }
    ChannelMode mode() { return m_mode; }

signals:

public slots:
    void startTest();
    void abortTest();
    void handleSerialResponseBundleReady(batlabPacketBundle bundle);

private:
    Batlab *m_batlab;

    ChannelMode m_mode;
    TestState m_test_state;

    std::time_t m_ts;

    int m_voltage_count;
    float m_voltage_avg;
    float m_voltage_prev;
    int m_voltage_error_count;

    int m_current_count;
    float m_current_avg;
    float m_current_prev;

    float m_vcc;

    float m_e;

    float m_temperature0;
    float m_delta_t;

    void periodicCheck();
    void handlePeriodicCheckResponse(QVector<BatlabPacket> response);
};

#endif // CHANNEL_H
