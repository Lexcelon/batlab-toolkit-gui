#ifndef BATLABPACKET_H
#define BATLABPACKET_H

class BatlabPacket
{
public:
    explicit BatlabPacket();
    explicit BatlabPacket(int batlabNamespace, int batlabRegister);
    // TODO
//    BatlabPacket readPacket(int batlabNamespace, int batlabRegister);
//    BatlabPacket writePacket(int batlabNamespace, int batlabRegister, uchar lowByte, uchar highByte);
//    BatlabPacket writePacket(int batlabNamespace, int batlabRegister, int payload);
    // void debug();
    int value();
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
