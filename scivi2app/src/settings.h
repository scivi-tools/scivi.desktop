#ifndef SETTINGS_H
#define SETTINGS_H

#include <QApplication>
#include <QCommandLineParser>
#include <QObject>

namespace scivi {

class Settings : public QObject
{
    static Settings *m_instance;
public:
    Settings(const QApplication &app);
    void fromCommandLineArguments(QCommandLineParser &cmdParser, const QStringList &arguments);

    QString filtersDir;
    QString pluginsDir;

    static Settings &getInstance();
};

Settings *getSettings();

}

#endif // SETTINGS_H
