#include <QApplication>
#include <QObject>
#include <QWidget>

#include "batlabmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BatlabMainWindow window;
    window.show();

    return app.exec();
}
