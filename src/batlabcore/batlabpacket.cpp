#include "batlabpacket.h"

BatlabPacket::BatlabPacket()
{
    // TODO
}

BatlabPacket::BatlabPacket(int batlabNamespace, int batlabRegister)
{
    m_startByte = static_cast<uchar>(0xAA);
    m_nameSpace = static_cast<uchar>(batlabNamespace);
    m_address = static_cast<uchar>(batlabRegister);
    m_payloadLowByte = static_cast<uchar>(0x00);
    m_payloadHighByte = static_cast<uchar>(0x00);
    m_writeTimeout_ms = DEFAULT_WRITE_TIMEOUT;
    m_readTimeout_ms = DEFAULT_READ_TIMEOUT;
    m_sleepAfterTransaction_ms = DEFAULT_SLEEP_AFTER_TRANSACTION;
    m_readVerify = false;
    m_retries = DEFAULT_SERIAL_RETRIES;
}

BatlabPacket::BatlabPacket(int batlabNamespace, int batlabRegister, int payloadLowByte, int payloadHighByte)
{
    m_startByte = static_cast<uchar>(0xAA);
    m_nameSpace = static_cast<uchar>(batlabNamespace);
    m_address = static_cast<uchar>(batlabRegister);
    m_payloadLowByte = static_cast<uchar>(payloadLowByte);
    m_payloadHighByte = static_cast<uchar>(payloadHighByte);
    m_writeTimeout_ms = DEFAULT_WRITE_TIMEOUT;
    m_readTimeout_ms = DEFAULT_READ_TIMEOUT;
    m_sleepAfterTransaction_ms = DEFAULT_SLEEP_AFTER_TRANSACTION;
    m_readVerify = false;
    m_retries = DEFAULT_SERIAL_RETRIES;
}

uchar BatlabPacket::getStartByte() { return m_startByte; }
void BatlabPacket::setStartByte(uchar startByte) { m_startByte = startByte; }

uchar BatlabPacket::getNamespace() { return m_nameSpace; }
void BatlabPacket::setNamespace(uchar ns) { m_nameSpace = ns; }

uchar BatlabPacket::getAddress() { return m_address; }
void BatlabPacket::setAddress(uchar address) { m_address = address; }

uchar BatlabPacket::getPayloadLowByte() { return m_payloadLowByte; }
void BatlabPacket::setPayloadLowByte(uchar lowByte) { m_payloadLowByte = lowByte; }

uchar BatlabPacket::getPayloadHighByte() { return m_payloadHighByte; }
void BatlabPacket::setPayloadHighByte(uchar highByte) { m_payloadHighByte = highByte; }

int BatlabPacket::getWriteTimeout_ms() { return m_writeTimeout_ms; }
void BatlabPacket::setWriteTimeout_ms(int writeTimeout_ms) { m_writeTimeout_ms = writeTimeout_ms; }

int BatlabPacket::getReadTimeout_ms()
{
    return m_readTimeout_ms;
}

int BatlabPacket::getSleepAfterTransaction_ms()
{
    return m_sleepAfterTransaction_ms;
}

void BatlabPacket::setSleepAfterTransaction_ms(int ms)
{
    m_sleepAfterTransaction_ms = ms;
}

bool BatlabPacket::getReadVerify()
{
    return m_readVerify;
}

int BatlabPacket::getRetries()
{
    return m_retries;
}

int BatlabPacket::value()
{
    return m_payloadLowByte + m_payloadHighByte*256;
}

//batlabPacket BatlabLib::writePacket(int batlabNamespace, int batlabRegister, uchar lowByte, uchar highByte)
//{
//    batlabPacket packet;
//    packet.startByte = static_cast<uchar>(0xAA);
//    packet.nameSpace = static_cast<uchar>(batlabNamespace);
//    packet.address = static_cast<uchar>(batlabRegister);
//    packet.payloadLowByte = lowByte;
//    packet.payloadHighByte = highByte;
//    packet.writeTimeout_ms = DEFAULT_WRITE_TIMEOUT;
//    packet.readTimeout_ms = DEFAULT_READ_TIMEOUT;
//    packet.sleepAfterTransaction_ms = DEFAULT_SLEEP_AFTER_TRANSACTION;
//    packet.readVerify = false;
//    packet.retries = DEFAULT_SERIAL_RETRIES;
//    return packet;
//}

//batlabPacket BatlabLib::writePacket(int batlabNamespace, int batlabRegister, int payload)
//{
//    uchar lowByte = static_cast<uchar>(payload & 0xff);
//    uchar highByte = static_cast<uchar>(payload >> 8);
//    return writePacket(batlabNamespace, batlabRegister, lowByte, highByte);
//}

//void BatlabLib::debugResponsePacket(int serialnumber, uchar packetStartByte, uchar packetNamespace, uchar packetAddress, uchar packetLowByte, uchar packetHighByte)
//{
//    qDebug() << "Response Packet:" << "Batlab S/N:" << qPrintable(QString::number(serialnumber).leftJustified(6, ' '))
//             << "Start Byte:"<< qPrintable("0x" + QString("%1").arg(packetStartByte, 0, 16).toUpper().rightJustified(2, '0'))
//             << "Namespace:" << qPrintable(BatlabLib::namespaceIntToString[packetNamespace].leftJustified(12, ' '))
//             << "Address:" << qPrintable("0x" + QString("%1").arg(packetAddress, 0, 16).toUpper().rightJustified(2, '0'))
//             << "Value:" << qPrintable("0x" + QString("%1").arg(256*packetHighByte + packetLowByte, 0, 16).toUpper().rightJustified(4, '0'));
//}
