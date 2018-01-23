#ifndef BATLABLOGGER_H
#define BATLABLOGGER_H

#include <QObject>

class BatlabLogger : public QObject
{
    Q_OBJECT

public:
    static BatlabLogger* instance();
    void handleQtMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

signals:
    void qtLogMessageReceived(QtMsgType, QString);

public slots:

private:
    BatlabLogger(){}
    BatlabLogger(BatlabLogger const&) = default;
    BatlabLogger& operator=(BatlabLogger const&) = default;
    static BatlabLogger* m_pInstance;
};

#endif // BATLABLOGGER_H
