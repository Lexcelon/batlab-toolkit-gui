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
#include "batlabcellmanager.h"
#include "globals.h"


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
    Ui::Batlab *ui = nullptr;
    QPushButton *test = nullptr;
    QPushButton *settingsB = nullptr;
    QPushButton *report = nullptr;
    QPushButton *options = nullptr;
    QPushButton *exit = nullptr;
    QGraphicsView *glWidget = nullptr;
    batlabtest *testObj = nullptr;
    batlabCom * com = nullptr;

    settings * configSettings = nullptr;

    batlabCellManager * cellManager = nullptr;

};

#endif // BATLAB_H
