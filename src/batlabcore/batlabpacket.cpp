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
int BatlabPacket::value() { return m_payloadLowByte + m_payloadHighByte*256; }

// TODO functions from python packet implementation

float BatlabPacket::asvoltage()
{
    if (std::isnan(value())) { return NAN; }  // LEFT OFF
}

//def asvoltage(self):
//    """Represents voltage ``data`` as a floating point voltage."""
//    if(math.isnan(self.data)):
//        return float(('nan'))
//    if(self.data & 0x8000): # the voltage can be negative
//        self.data = -0x10000 + self.data
//    flt = float(self.data * 4.5 / 2**15)
//    return flt

//def asvcc(self):
//    """Represents vss ``data`` as a floating point voltage."""
//    return 2**15 * 4.096 / self.data

//def asfreq(self):
//    """Represents frequency data in Hz."""
//    return self.data * (10000.0 / 256.0)

//def asioff(self):
//    """Represents register current to floating point Amps."""
//    return self.data / 128.0

//def assetpoint(self):
//    """Represents current setpoint as floating point Amps."""
//    return self.data / 128.0

//def asmagdiv(self):
//    """Represents magdiv register as Ipp."""
//    return 2.0 / (2 ** self.data)

//def asmode(self):
//    """Represents a mode register value as an enum string."""
//    try:
//        return MODE_LIST[self.data]
//    except:
//        return 'MODE_UNKNOWN: ' + str(self.data)

//def aserr(self):
//    """Represents error reg bit field as a string of the error flags."""
//    if(math.isnan(self.data)):
//        return 'ERR_NONE'
//    for i in range(0,len(ERR_LIST)):
//        if self.data & (1 << i):
//            return ERR_LIST[i]
//    return 'ERR_NONE'

//def astemperature(self):
//    try:
//        Rdiv = self.R[self.namespace]
//        R = Rdiv / ((2**15 / self.data)-1)
//        To = 25 + 273.15
//        Ro = 10000
//        B = self.B[self.namespace] # 3380
//        Tinv = (1 / To) + (math.log(R/Ro) / B)
//        T = (1 / Tinv) - 273.15
//        T = (T * 1.8) + 32
//    except:
//        T = float('nan')
//    return T

//def astemperature(self,Rlist,Blist):
//    """Represents temp data as temperature in F.
//    Args:
//        Rlist: 4 list of 'R' calibration values needed to interpret temp
//        Blist: 4 list of 'B' calibration values needed to interpret temp
//    """
//    try:
//        Rdiv = Rlist[self.namespace]
//        R = Rdiv / ((2**15 / self.data)-1)
//        To = 25 + 273.15
//        Ro = 10000
//        B = Blist[self.namespace] # 3380
//        Tinv = (1 / To) + (math.log(R/Ro) / B)
//        T = (1 / Tinv) - 273.15
//        T = (T * 1.8) + 32

//    except:
//        T = float('nan')
//    return T

//def astemperature_c(self,Rlist,Blist):
//    """Represents temp data as temperature in C.
//    Args:
//        Rlist: 4 list of 'R' calibration values needed to interpret temp
//        Blist: 4 list of 'B' calibration values needed to interpret temp
//    """
//    try:
//        Rdiv = Rlist[self.namespace]
//        R = Rdiv / ((2**15 / self.data)-1)
//        To = 25 + 273.15
//        Ro = 10000
//        B = Blist[self.namespace] # 3380
//        Tinv = (1 / To) + (math.log(R/Ro) / B)
//        T = (1 / Tinv) - 273.15
//    except:
//        T = float('nan')
//    return T

//def ascurrent(self):
//    if(math.isnan(self.data)):
//        return float(('nan'))
//    """Represents current measurement as float current in Amps."""
//    if(self.data & 0x8000): # the current can be negative
//        self.data = -0x10000 + self.data
//    return self.data * 4.096 / 2**15

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
