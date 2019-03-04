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

BatlabPacket::BatlabPacket(int batlabNamespace, int batlabRegister, uchar payloadLowByte, uchar payloadHighByte)
{
    m_startByte = static_cast<uchar>(0xAA);
    m_nameSpace = static_cast<uchar>(batlabNamespace);
    m_address = static_cast<uchar>(batlabRegister | 0x80);
    m_payloadLowByte = static_cast<uchar>(payloadLowByte);
    m_payloadHighByte = static_cast<uchar>(payloadHighByte);
    m_writeTimeout_ms = DEFAULT_WRITE_TIMEOUT;
    m_readTimeout_ms = DEFAULT_READ_TIMEOUT;
    m_sleepAfterTransaction_ms = DEFAULT_SLEEP_AFTER_TRANSACTION;
    m_readVerify = false;
    m_retries = DEFAULT_SERIAL_RETRIES;
}

BatlabPacket::BatlabPacket(int batlabNamespace, int batlabRegister, uint16_t payload)
{
    m_startByte = static_cast<uchar>(0xAA);
    m_nameSpace = static_cast<uchar>(batlabNamespace);
    m_address = static_cast<uchar>(batlabRegister | 0x80);
    m_payloadLowByte = static_cast<uchar>(payload & 0xff);
    m_payloadHighByte = static_cast<uchar>(payload >> 8);
    m_writeTimeout_ms = DEFAULT_WRITE_TIMEOUT;
    m_readTimeout_ms = DEFAULT_READ_TIMEOUT;
    m_sleepAfterTransaction_ms = DEFAULT_SLEEP_AFTER_TRANSACTION;
    m_readVerify = false;
    m_retries = DEFAULT_SERIAL_RETRIES;
}

void BatlabPacket::debug()
{
    qDebug() << "Response packet:\n"
             << "    " << QString("%1").arg(m_startByte , 0, 16) << "\n"
             << "    " << QString("%1").arg(m_nameSpace , 0, 16) << "\n"
             << "    " << QString("%1").arg(m_address , 0, 16) << "\n"
             << "    " << QString("%1").arg(m_payloadLowByte , 0, 16) << "\n"
             << "    " << QString("%1").arg(m_payloadHighByte , 0, 16) << "\n";
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

quint16 BatlabPacket::getValue() { return m_payloadLowByte + m_payloadHighByte*256; }
void BatlabPacket::setValue(quint16 value)
{
    m_payloadHighByte = value >> 8;
    m_payloadLowByte = value & 0xFF;
}

// Represents voltage data as a floating point voltage
float BatlabPacket::asVoltage()
{
    if (std::isnan((float)getValue())) { return NAN; }
    if (getValue() & 0x8000)  // Voltage can be negative
    {
        setValue(-0x10000 + getValue());
    }
    return getValue() * 4.5 / pow(2, 15);
}

// Represents vss data as a floating point voltage
float BatlabPacket::asVcc()
{
    return pow(2, 15) * 4.096 / getValue();
}

// Represents frequency data in Hz
float BatlabPacket::asFreq()
{
    return getValue() * (10000.0 / 256.0);
}

// Represents register current to floating point Amps
float BatlabPacket::asIOff()
{
    return getValue() / 128.0;
}

// Represents current setpoint as floating point Amps
float BatlabPacket::asSetPoint()
{
    return getValue() / 128.0;
}

// Represents magdiv register as Ipp
float BatlabPacket::asMagdiv()
{
    return 2.0 / pow(2, getValue());
}

QString BatlabPacket::asMode()
{
    //    Represents a mode register value as an enum string
    //    try:
    //        return MODE_LIST[self.data]
    //    except:
    //        return 'MODE_UNKNOWN: ' + str(self.data)
    return "";
}

QString BatlabPacket::asErr()
{
    //    Represents error reg bit field as a string of the error flags
    //    if(math.isnan(self.data)):
    //        return 'ERR_NONE'
    //    for i in range(0,len(ERR_LIST)):
    //        if self.data & (1 << i):
    //            return ERR_LIST[i]
    //    return 'ERR_NONE'
    return "";
}

// Represents data as temperature in F
// Rlist: 4 list of 'R' calibration values needed to interpret temp
// Blist: 4 list of 'B' calibration values needed to interpret temp
float BatlabPacket::asTemperatureF(QVector<int> RList, QVector<int> BList)
{
    float T;
    try
    {
        int Rdiv = RList[(int)getNamespace()];
        float R = Rdiv / ((pow(2, 15) / getValue()) - 1);
        float To = 25 + 273.15;
        float Ro = 10000;
        int B = BList[(int)getNamespace()];  // 3380
        float Tinv = (1 / To) + (log(R / Ro) / B);
        T = (1 / Tinv) - 273.15;
        T = (T * 1.8) + 32;
    }
    catch (...)
    {
        T = NAN;
    }
    return T;
}

// Represents data as temperature in C
// Rlist: 4 list of 'R' calibration values needed to interpret temp
// Blist: 4 list of 'B' calibration values needed to interpret temp
float BatlabPacket::asTemperatureC(QVector<int> RList, QVector<int> BList)
{
    float T;
    try
    {
        int Rdiv = RList[(int)getNamespace()];
        float R = Rdiv / ((pow(2, 15) / getValue()) - 1);
        float To = 25 + 273.15;
        float Ro = 10000;
        int B = BList[(int)getNamespace()];  // 3380
        float Tinv = (1 / To) + (log(R / Ro) / B);
        T = (1 / Tinv) - 273.15;
    }
    catch (...)
    {
        T = NAN;
    }
    return T;
}

// Represents current measurement as float current in Amps
float BatlabPacket::asCurrent()
{
    if (std::isnan((float)getValue())) { return NAN; }
    if (getValue() & 0x8000)  // Voltage can be negative
    {
        setValue(-0x10000 + getValue());
    }
    return getValue() * 4.096 / pow(2, 15);
}

//def print_packet(self):
//    if(self.type == 'RESPONSE'):
//        if self.write == True:
//            logging.info("Wrote: Cell "+str(self.namespace)+", Addr "+"{0:#4X}".format(self.addr & 0x7F))
//        else:
//            logging.info("Read: Cell "+str(self.namespace)+", Addr "+"{0:#4X}".format(self.addr & 0x7F)+": "+str(self.data))

//def display(self):
//    """Prints out the basic info about the packet transaction ### charge function."""
//    if(self.type == 'RESPONSE'):
//        if self.write == True:
//            print('Wrote: Cell '+str(self.namespace)+', Addr '+"{0:#4X}".format(self.addr & 0x7F))
//        else:
//print('Read: Cell '+str(self.namespace)+', Addr '+"{0:#4X}".format(self.addr & 0x7F)+': '+str(self.data))
