#include "batlab.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Batlab w;
    w.show();

    return a.exec();
}
