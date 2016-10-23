#include "batlab.h"
#include <QApplication>
#include "batlabcom.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

   // batlabCom com;
     Batlab w;
    w.show();

//    com.onGetTemp(0,0);
//    volatile int i;
//    for (i = 0; i < 10000000; ++i) {

//        ;
//    }
//    com.onGetCharge(0,0);
//    for (i = 0; i < 10000000; ++i) {

//        ;
//    }
//    com.onGetCurrAmp(0,0);
//    for (i = 0; i < 10000000; ++i) {

//        ;
//    }
//    com.onGetVolAmp(0,0);
//    for (i = 0; i < 10000000; ++i) {

//        ;
//    }
//    com.onGetVolPhase(0,0);
//    for (i = 0; i < 10000000; ++i) {

//        ;
//    }
//    com.onGetVol(0,0);
//    for (i = 0; i < 10000000; ++i) {

//        ;
//    }
//    com.onGetCurr(0,0);
//    for (i = 0; i < 10000000; ++i) {

//        ;
//    }
//    com.onGetEnergy(0,0);
//    for (i = 0; i < 10000000; ++i) {

//        ;
//    }

    return a.exec();
}
