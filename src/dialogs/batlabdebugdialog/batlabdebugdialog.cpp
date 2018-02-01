#include "batlabdebugdialog.h"
#include "ui_batlabdebugdialog.h"

BatlabDebugDialog::BatlabDebugDialog(QWidget *parent, QVector<batlabDisplayInfo> infos) :
    QDialog(parent),
    ui(new Ui::debugDialog)
{
    ui->setupUi(this);

    // TODO and use qint16 and qint8 instead of int

    QLabel* registerReadNamespaceLabel = new QLabel(tr("Namespace:"));
    registerReadNamespaceSpinbox = new HexSpinBox(true);
    registerReadNamespaceSpinbox->setMaximum(0xFF);
    QLabel* registerReadAddressLabel = new QLabel(tr("Register Address:"));
    registerReadAddressSpinbox = new HexSpinBox(true);
    registerReadAddressSpinbox->setMaximum(0x80);
    registerReadButton = new QPushButton(tr("Register Read"));

    QGridLayout* registerReadLayout = new QGridLayout;
    registerReadLayout->addWidget(registerReadNamespaceLabel, 0, 0);
    registerReadLayout->addWidget(registerReadNamespaceSpinbox, 0, 1);
    registerReadLayout->addWidget(registerReadAddressLabel, 0, 2);
    registerReadLayout->addWidget(registerReadAddressSpinbox, 0, 3);
    registerReadLayout->addWidget(registerReadButton, 1, 0, 1, 4);

    ui->registerReadGroupBox->setLayout(registerReadLayout);

    QLabel* registerWriteNamespaceLabel = new QLabel(tr("Namespace:"));
    registerWriteNamespaceSpinbox = new HexSpinBox(true);
    registerWriteNamespaceSpinbox->setMaximum(0xFF);
    QLabel* registerWriteAddressLabel = new QLabel(tr("Register Address:"));
    registerWriteAddressSpinbox = new HexSpinBox(true);
    registerWriteAddressSpinbox->setMaximum(0x80);
    QLabel* registerWriteValueLabel = new QLabel(tr("Write Value:"));
    registerWriteValueSpinbox = new HexSpinBox(true);
    registerWriteValueSpinbox->setMaximum(0xFF);
    registerWriteButton = new QPushButton(tr("Register Write"));

    QGridLayout* registerWriteLayout = new QGridLayout;
    registerWriteLayout->addWidget(registerWriteNamespaceLabel, 0, 0);
    registerWriteLayout->addWidget(registerWriteNamespaceSpinbox, 0, 1);
    registerWriteLayout->addWidget(registerWriteAddressLabel, 0, 2);
    registerWriteLayout->addWidget(registerWriteAddressSpinbox, 0, 3);
    registerWriteLayout->addWidget(registerWriteValueLabel, 1, 0);
    registerWriteLayout->addWidget(registerWriteValueSpinbox, 1, 1);
    registerWriteLayout->addWidget(registerWriteButton, 2, 0, 1, 4);

    ui->registerWriteGroupBox->setLayout(registerWriteLayout);

    for (int i = 0; i < infos.size(); i++)
    {
        ui->selectBatlabComboBox->addItem(QString::number(infos.at(i).serialNumberComplete));
    }

    connect(registerReadButton, &QPushButton::clicked, this, &BatlabDebugDialog::processRegisterReadClick);
    connect(registerWriteButton, &QPushButton::clicked, this, &BatlabDebugDialog::processRegisterWriteClick);
}

BatlabDebugDialog::~BatlabDebugDialog()
{
    delete ui;
}

void BatlabDebugDialog::processRegisterReadClick() {
    emit registerReadRequested(ui->selectBatlabComboBox->currentText().toInt(), registerReadNamespaceSpinbox->value(), registerReadAddressSpinbox->value());
}

void BatlabDebugDialog::processRegisterWriteClick() {
    emit registerWriteRequested(ui->selectBatlabComboBox->currentText().toInt(), registerWriteNamespaceSpinbox->value(), registerWriteAddressSpinbox->value(), registerWriteValueSpinbox->value());
}
