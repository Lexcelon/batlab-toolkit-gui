#ifndef BATLABWIZARD_H
#define BATLABWIZARD_H

#include <QObject>
#include <QWizard>
#include <QWizardPage>
#include "../globals.h"

#include "wizardpageone.h"
#include "wizardpagetwo.h"
#include "wizardpagethree.h"

class newTestScheduleWizard : public QObject
{
    Q_OBJECT
public:
    explicit newTestScheduleWizard(QObject *parent = 0);
    ~newTestScheduleWizard();

signals:

    void emitFinished(QString projectName);

public slots:
    void onShow();

    void onIDChanged();


private:
//    wizardPageOne * page1;
//    wizardPageTwo * page2;
//    wizardPageThree * page3;
    QWizard * wiz;
};

#endif // BATLABWIZARD_H
