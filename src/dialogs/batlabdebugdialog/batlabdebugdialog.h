#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include "batlabcore/batlab.h"
#include "components/hexspinbox.h"

// TODO review this implementation

namespace Ui {
class debugDialog;
}

class BatlabDebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatlabDebugDialog(QWidget *parent = 0, QVector<batlabDisplayInfo> infos = QVector<batlabDisplayInfo>());
    ~BatlabDebugDialog();

public slots:
    void processRegisterReadClick();
    void processRegisterWriteClick();

signals:
    void registerReadRequested(int, int, int);
    void registerWriteRequested(int, int, int, int);

private:
    Ui::debugDialog *ui;

    HexSpinBox* registerReadNamespaceSpinbox;
    HexSpinBox* registerReadAddressSpinbox;
    QPushButton* registerReadButton;

    HexSpinBox* registerWriteNamespaceSpinbox;
    HexSpinBox* registerWriteAddressSpinbox;
    HexSpinBox* registerWriteValueSpinbox;
    QPushButton* registerWriteButton;

};

#endif // TEST_H
