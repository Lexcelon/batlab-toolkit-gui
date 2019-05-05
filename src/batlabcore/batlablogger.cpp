#include "batlablogger.h"

BatlabLogger *BatlabLogger::m_pInstance = NULL;

BatlabLogger *BatlabLogger::instance() {
  qRegisterMetaType<QtMsgType>("QtMsgType");
  if (!m_pInstance) {
    m_pInstance = new BatlabLogger;
  }
  return m_pInstance;
}

void BatlabLogger::handleQtMessage(QtMsgType type,
                                   const QMessageLogContext &context,
                                   const QString &msg) {
  emit qtLogMessageReceived(type, msg);

  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
    fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtInfoMsg:
    fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtWarningMsg:
    fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtCriticalMsg:
    fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    break;
  case QtFatalMsg:
    fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
    abort();
  }
}
