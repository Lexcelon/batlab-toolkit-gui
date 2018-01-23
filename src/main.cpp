#include <QApplication>
#include <QObject>
#include <QWidget>

#include "batlabmainwindow.h"

int main(int argc, char *argv[])
{
    // Custom handler of messages in Qt. We use our own function to display these on the live view as well as standard output.
    qInstallMessageHandler(myMessageHandler);

    QApplication app(argc, argv);

    BatlabMainWindow window;
    window.show();

    return app.exec();
}
