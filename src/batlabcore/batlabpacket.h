#ifndef BATLABPACKET_H
#define BATLABPACKET_H

#include <QObject>

#include "batlablib.h"

class BatlabPacket
{
public:
    explicit BatlabPacket();
    explicit BatlabPacket(int batlabNamespace, int batlabRegister);  // Read packet
    explicit BatlabPacket(int batlabNamespace, int batlabRegister, int payload);  // Write packet
    explicit BatlabPacket(int batlabNamespace, int batlabRegister, int payloadLowByte, int payloadHighByte);  // Write packet

    int value();
    uchar getStartByte();
    void setStartByte(uchar startByte);
    uchar getNamespace();
    void setNamespace(uchar ns);
    uchar getAddress();
    void setAddress(uchar address);
    uchar getPayloadLowByte();
    void setPayloadLowByte(uchar lowByte);
    uchar getPayloadHighByte();
    void setPayloadHighByte(uchar highByte);
    int getWriteTimeout_ms();
    void setWriteTimeout_ms(int writeTimeout_ms);
    int getReadTimeout_ms();
    int getSleepAfterTransaction_ms();
    void setSleepAfterTransaction_ms(int ms);
    bool getReadVerify();
    int getRetries();
signals:

public slots:

private:
    uchar m_startByte;
    uchar m_nameSpace;
    uchar m_address;
    uchar m_payloadLowByte;
    uchar m_payloadHighByte;
    int m_writeTimeout_ms;
    int m_readTimeout_ms;
    int m_sleepAfterTransaction_ms;
    bool m_readVerify;
    int m_retries;
};

struct batlabPacketBundle {
    QQueue<BatlabPacket> packets;
    QString callback;
    int channel;  // -1 if not for specific channel
};

#endif // BATLABPACKET_H
