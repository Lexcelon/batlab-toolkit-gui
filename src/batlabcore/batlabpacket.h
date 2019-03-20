#ifndef BATLABPACKET_H
#define BATLABPACKET_H

#include <QObject>

#include "batlablib.h"

class BatlabPacket
{
public:
    explicit BatlabPacket();
    explicit BatlabPacket(int batlabNamespace, int batlabRegister);  // Read packet
    explicit BatlabPacket(int batlabNamespace, int batlabRegister, uint16_t payload);  // Write packet
    explicit BatlabPacket(int batlabNamespace, int batlabRegister, uchar payloadLowByte, uchar payloadHighByte);  // Write packet

    quint16 getValue();
    BatlabPacket setValue(quint16 value);

    uchar getStartByte();
    BatlabPacket setStartByte(uchar startByte);
    uchar getNamespace();
    BatlabPacket setNamespace(uchar ns);
    uchar getAddress();
    BatlabPacket setAddress(uchar address);
    uchar getPayloadLowByte();
    BatlabPacket setPayloadLowByte(uchar lowByte);
    uchar getPayloadHighByte();
    BatlabPacket setPayloadHighByte(uchar highByte);
    int getWriteTimeout_ms();
    BatlabPacket setWriteTimeout_ms(int writeTimeout_ms);
    int getReadTimeout_ms();
    int getSleepAfterTransaction_ms();
    BatlabPacket setSleepAfterTransaction_ms(int ms);
    bool getReadVerify();
    int getRetries();

    float asVoltage();
    float asVcc();
    float asFreq();
    float asIOff();
    float asSetPoint();
    float asMagdiv();
    QString asMode();
    QString asErr();
    float asTemperatureF(int Rdiv, int B);
    float asTemperatureF(QVector<int> RList, QVector<int> BList);
    float asTemperatureC(int Rdiv, int B);
    float asTemperatureC(QVector<int> RList, QVector<int> BList);
    float asCurrent();

    void debug();
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
    QVector<BatlabPacket> packets;
    QString callback;
    int channel;  // -1 if not for specific channel
};

#endif // BATLABPACKET_H
