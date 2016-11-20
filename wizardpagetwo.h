#ifndef WIZARDPAGETWO_H
#define WIZARDPAGETWO_H

#include <QWidget>

namespace Ui {
class wizardPageTwo;
}

class wizardPageTwo : public QWidget
{
    Q_OBJECT

public:
    explicit wizardPageTwo(QWidget *parent = 0);
    ~wizardPageTwo();

private:
    Ui::wizardPageTwo *ui;
};

#endif // WIZARDPAGETWO_H
