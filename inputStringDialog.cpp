#include "inputStringDialog.h"
#include "ui_inputStringDialog.h"

inputStringDialog::inputStringDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputStringDialog)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));
}

inputStringDialog::~inputStringDialog()
{
    delete ui;
}

void inputStringDialog::onStringList(QStringList stringList)
{
    ui->listWidget->addItems(stringList);
}

void inputStringDialog::onAccepted()
{
    QStringList list;
    for (int i = 0; i < ui->listWidget->size(); ++i) {
        if (ui->listWidget->item(i)->isSelected()) {
            list << ui->listWidget->item(i)->text();
        }
    }
    emit emitList(list);
}
