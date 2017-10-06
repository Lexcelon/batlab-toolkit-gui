#ifndef WIZARDPAGEONE_H
#define WIZARDPAGEONE_H

#include <QWidget>

namespace Ui {
class wizardPageOne;
}

class wizardPageOne : public QWidget
{
    Q_OBJECT

public:
    explicit wizardPageOne(QWidget *parent = 0);
    ~wizardPageOne();

public slots:
    void onFinish();

    void onDesignator();
    void onStartInd();
    void onNumCells(int val);
    void onNew(bool val);
    void onProjectName();

    void onUpdate();

signals:
    void emitDesignator(QString);
    void emitStartInd(QString);
    void emitProjectName(QString);
    void emitNumCells(int);
    void emitIsNew(bool);

private:
    Ui::wizardPageOne *ui;
    QString name;
    QString startInd;
    int numCells;
    bool isNew;
    QString projectName;
};

#endif // WIZARDPAGEONE_H