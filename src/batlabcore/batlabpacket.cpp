#include "batlabpacket.h"

BatlabPacket::BatlabPacket()
{
    m_startByte = static_cast<uchar>(0xAA);
    m_nameSpace = static_cast<uchar>(0x00);
    m_address = static_cast<uchar>(0x00);
    m_payloadLowByte = static_cast<uchar>(0x00);
    m_payloadHighByte = static_cast<uchar>(0x00);
    m_writeTimeout_ms = DEFAULT_WRITE_TIMEOUT;
    m_readTimeout_ms = DEFAULT_READ_TIMEOUT;
    m_sleepAfterTransaction_ms = DEFAULT_SLEEP_AFTER_TRANSACTION;
    m_readVerify = false;
    m_retries = DEFAULT_SERIAL_RETRIES;
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

BatlabPacket::BatlabPacket(int batlabNamespace, int batlabRegister, int payload)
{
    m_startByte = static_cast<uchar>(0xAA);
    m_nameSpace = static_cast<uchar>(batlabNamespace);
    m_address = static_cast<uchar>(batlabRegister);
    m_payloadLowByte = static_cast<uchar>(payload & 0xff);
    m_payloadHighByte = static_cast<uchar>(payload >> 8);
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

int BatlabPacket::getReadTimeout_ms() { return m_readTimeout_ms; }
int BatlabPacket::getSleepAfterTransaction_ms() { return m_sleepAfterTransaction_ms; }

void BatlabPacket::setSleepAfterTransaction_ms(int ms) { m_sleepAfterTransaction_ms = ms; }
bool BatlabPacket::getReadVerify() { return m_readVerify; }

int BatlabPacket::getRetries() { return m_retries; }
int BatlabPacket::value() { return m_payloadLowByte + m_payloadHighByte*256; }

// TODO functions from python packet implementation
