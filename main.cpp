#include "BatlabApp.h"
#include <QApplication>
#include <QObject>
#include <QWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create the gui and show it
    BatlabApp w;
    w.show();

    return a.exec();
}
