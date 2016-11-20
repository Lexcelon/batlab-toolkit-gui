#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>

namespace Ui {
class testWindow;
}

class testWindow : public QDialog
{
    Q_OBJECT

public:
    explicit testWindow(QWidget *parent = 0);
    ~testWindow();

private:
    Ui::testWindow *ui;
};

#endif // TESTWINDOW_H
