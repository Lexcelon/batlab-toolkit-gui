#include "batlab.h"
#include <QApplication>
#include "batlabcell.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Batlab w;
    w.show();

//    batlabCell* b = new batlabCell();
//    for (int i = 0; i < b->getTemperature()->size(); ++i) {
//        qDebug() << b->getTemperature()->at(i);
//    }

    return a.exec();
}
