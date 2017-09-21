#ifndef CELLMODULEWIDGET_H
#define CELLMODULEWIDGET_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class CellModuleWidget;
}

class CellModuleWidget : public QWidget
{
    Q_OBJECT

public:
    CellModuleWidget(QWidget *parent = 0);
    CellModuleWidget(int moduleNumber, QStringList cellList, QWidget *parent = 0);
    ~CellModuleWidget();

private:
    Ui::CellModuleWidget *ui;
};

#endif // CELLMODULEWIDGET_H
