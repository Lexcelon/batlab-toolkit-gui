#ifndef INPUTSTRINGDIALOG_H
#define INPUTSTRINGDIALOG_H

#include <QDialog>

namespace Ui {
class inputStringDialog;
}

class inputStringDialog : public QDialog
{
    Q_OBJECT

public:
    explicit inputStringDialog(QWidget *parent = 0);
    ~inputStringDialog();

public slots:

    void onStringList(QStringList stringList);
    void onAccepted();

signals:
    void emitList(QStringList);

private:
    Ui::inputStringDialog *ui;
};

#endif // INPUTSTRINGDIALOG_H
