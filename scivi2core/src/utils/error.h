#ifndef ERROR_H
#define ERROR_H

#include <QString>

class Error {
public:
    explicit Error(const std::string &what_arg)
        : m_msg{QString::fromStdString(what_arg)} {}
    explicit Error(const char *what_arg) : m_msg{QString(what_arg)} {}
    explicit Error(const QString &msg_arg) : m_msg{msg_arg} {}
    QString msg();

private:
    QString m_msg{""};
};

#endif  // ERROR_H
