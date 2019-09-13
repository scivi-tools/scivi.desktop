#include "settings.h"

namespace scivi {

Settings *Settings::m_instance = nullptr;

Settings::Settings(const QApplication &app)
    : QObject(nullptr), filtersDir(app.applicationDirPath()), pluginsDir(app.applicationDirPath())
{
    m_instance = this;
}

void Settings::fromCommandLineArguments(QCommandLineParser &cmdParser, const QStringList &arguments)
{
    QCommandLineOption pluginsDirOpt(QStringList() << "p" << "plugins_dir", tr("Path to plugin implementations."), "dir");
    QCommandLineOption filtersDirOpt(QStringList() << "f" << "filters_dir", tr("Path to filter implementations."), "dir");
    cmdParser.addOptions({ pluginsDirOpt, filtersDirOpt });
    cmdParser.process(arguments);
    if (cmdParser.isSet(pluginsDirOpt)) {
        pluginsDir = cmdParser.value(pluginsDirOpt);
    }
    if (cmdParser.isSet(filtersDirOpt)) {
        filtersDir = cmdParser.value(filtersDirOpt);
    }
}

Settings& Settings::getInstance()
{
    return *m_instance;
}

Settings *getSettings()
{
    return &Settings::getInstance();
}

}
