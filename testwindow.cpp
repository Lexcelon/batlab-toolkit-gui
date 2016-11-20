#include "testwindow.h"
#include "ui_testwindow.h"

testWindow::testWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::testWindow)
{
    ui->setupUi(this);
}

testWindow::~testWindow()
{
    delete ui;
}
