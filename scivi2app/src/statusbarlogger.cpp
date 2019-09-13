#include "statusbarlogger.h"

#include <QDateTime>
#include <QJsonDocument>

StatusBarLogger::StatusBarLogger(): QObject()
{
}

QString msgTypeToString(QtMsgType type) {
    switch (type) {
    case QtMsgType::QtInfoMsg:
        return "info";
    case QtMsgType::QtDebugMsg:
        return "debug";
    case QtMsgType::QtFatalMsg:
        return "fatal";
    case QtMsgType::QtWarningMsg:
        return "warning";
    case QtMsgType::QtCriticalMsg:
        return "critical";
    }
}

struct LogEntry {
    QDateTime timestamp;
    QtMsgType lvl;
    QString msg;

    QJsonDocument toJson() {
        QJsonObject obj;
        obj["m"] = msg;
        obj["l"] = msgTypeToString(lvl);
        obj["d"] = timestamp.toString(Qt::ISODate);
        return QJsonDocument(obj);
    }
};

void StatusBarLogger::messageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    Q_UNUSED(ctx);
    LogEntry entry;
    entry.msg = msg;
    entry.timestamp = QDateTime::currentDateTime();
    entry.lvl = type;

    QJsonObject obj;
    FILE* stream { nullptr };
    bool show = false;
    switch (type) {
    case QtMsgType::QtWarningMsg:
        stream = stderr;
        break;
    case QtMsgType::QtCriticalMsg:
    case QtMsgType::QtFatalMsg:
        stream = stderr;
        show = true;
        break;
    case QtMsgType::QtDebugMsg:
        stream = stdout;
        break;
    case QtMsgType::QtInfoMsg:
        stream = stdout;
        show = true;
        break;
    }
#ifdef QT_DEBUG
    fprintf(stream, "%s\n", entry.toJson().toJson(QJsonDocument::JsonFormat::Compact).toStdString().c_str());
    fflush(stream);
#endif
    if (show) {
        instance().m_msg = msg;
        instance().m_msgType = msgTypeToString(type);
        emit instance().msgChanged();
        emit instance().msgTypeChanged();
    }
}
