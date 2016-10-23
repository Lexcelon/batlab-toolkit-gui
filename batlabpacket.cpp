#include "batlabpacket.h"

batlabPacket::batlabPacket() : QObject()
{

}

batlabPacket::~batlabPacket()
{

}



void batlabPacket::printThis() {
    if (type == "RESPONSE") {
        if (write == true) {
            qDebug() << "Unit: " << unit << " Cell: " << cell << " Addr: " << QString::number(addr&0x7f,16);
        } else {
            qDebug() << "Unit: " << unit << " Cell: " << cell << " Addr: " << QString::number(addr&0x7f);
        }
    }
    if (type == "STREAM") {
        qDebug() << "Unit " << unit << ", Cell " << cell << ":";
    }
}
