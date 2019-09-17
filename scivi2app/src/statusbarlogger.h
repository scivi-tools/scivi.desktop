#ifndef STATUSBARLOGGER_H
#define STATUSBARLOGGER_H

#include <QObject>
#include "singleton.h"

class StatusBarLogger : public QObject, public Singleton<StatusBarLogger> {
    Q_OBJECT
    Q_PROPERTY(QString msg READ msg NOTIFY msgChanged)
    Q_PROPERTY(QString msgType READ msgType NOTIFY msgTypeChanged)
public:
    StatusBarLogger();
    static void messageHandler(QtMsgType type, const QMessageLogContext &ctx,
                               const QString &msg);

    QString msg() const { return m_msg; }
    QString msgType() const { return m_msgType; }

signals:
    void msgChanged();
    void msgTypeChanged();

private:
    QString m_msg = {};
    QString m_msgType = {};
};

#endif  // STATUSBARLOGGER_H
