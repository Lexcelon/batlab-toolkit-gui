#ifndef TEST_H
#define TEST_H

#include <QComboBox>
#include <QDialog>

#include "batlabcore/batlab.h"
#include "components/hexspinbox.h"

namespace Ui {
class debugDialog;
}

class BatlabDebugDialog : public QDialog {
  Q_OBJECT

public:
  explicit BatlabDebugDialog(
      QWidget *parent = nullptr,
      QVector<batlabStatusInfo> infos = QVector<batlabStatusInfo>(),
      QVector<QString> firmwareVersions = QVector<QString>());
  ~BatlabDebugDialog();

public slots:
  void processRegisterReadClick();
  void processRegisterWriteClick();
  void processFirmwareFlashClick();

  void updateInfo(QVector<batlabStatusInfo> infos,
                  QVector<QString> firmwareVersions);
  void updateBatlabs(QVector<batlabStatusInfo> infos);

signals:
  void registerReadRequested(int serialNumber, int name_space, int address);
  void registerWriteRequested(int serialNumber, int name_space, int address,
                              int value);
  void firmwareFlashRequested(int serialNumber, QString firmwareVersion,
                              QString previousFirmwareVersion);

private:
  Ui::debugDialog *ui;

  HexSpinBox *registerReadNamespaceSpinbox;
  HexSpinBox *registerReadAddressSpinbox;
  QPushButton *registerReadButton;

  HexSpinBox *registerWriteNamespaceSpinbox;
  HexSpinBox *registerWriteAddressSpinbox;
  HexSpinBox *registerWriteValueSpinbox;
  QPushButton *registerWriteButton;

  QComboBox *flashFirmwareComboBox;
  QPushButton *flashFirmwareButton;
};

#endif // TEST_H
