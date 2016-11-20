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

private:
    Ui::wizardPageOne *ui;
};

#endif // WIZARDPAGEONE_H
