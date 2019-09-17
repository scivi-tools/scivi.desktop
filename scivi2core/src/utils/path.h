#ifndef PATH_H
#define PATH_H

#include <QRegularExpression>
#include <QString>

namespace PathUtils {

static QString removeSchema(const QString &url) {
    QRegularExpression re("^(file:/{2})|(qrc:/{2})|(http:/{2})");
    QString res = url;
    res.replace(re, QStringLiteral(""));
#ifdef Q_OS_WIN
    res.replace(QRegularExpression("^/"), QStringLiteral(""));
#endif
    return res;
}

}  // namespace PathUtils

#endif  // PATH_H
