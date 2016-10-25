#ifndef BATLAB_H
#define BATLAB_H

#include <QMainWindow>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGraphicsView>
#include "batlabconfig.h"
#include "settings.h"
#include "bldataobject.h"
#include "test.h"
#include "batlabcom.h"
#include "globals.h"

static QString parseStatus(int status) {
    QString str;
    for (int i = 0; i < statusString.size(); ++i) {
        if (status & (1<<i)) {
                str = str + " " + statusString[1<<i];
        }
    }
    return str;
}

namespace Ui {
class Batlab;
}

class Batlab : public QMainWindow
{
    Q_OBJECT

public:
    explicit Batlab(QWidget *parent = 0);
    ~Batlab();

public slots:
    void onTest();
    void onReceiveResponse(int,int,QString,int);
    void onReceiveStream(int,int,int,float,int,int,int);
    void onReceiveStreamExt(int,int,int,int,int);


private:
    Ui::Batlab *ui;
    QPushButton *test;
    QPushButton *settingsB;
    QPushButton *report;
    QPushButton *options;
    QPushButton *exit;
    QGraphicsView *glWidget;
    batlabtest *testObj = nullptr;
    batlabCom * com = nullptr;

    settings * configSettings;

};

#endif // BATLAB_H
