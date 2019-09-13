#ifndef SCIVIAPPLICATION_H
#define SCIVIAPPLICATION_H

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSharedPointer>
#include "pluginmanager.h"
#include "editor/editor.h"
#include "settings.h"

#if defined Q_OS_BLACKBERRY || defined Q_OS_ANDROID || defined Q_OS_IOS || defined Q_OS_WP
#define Q_OS_MOBILE
#else
#define Q_OS_DESKTOP
#endif

#define APPLICATION_NAME QStringLiteral("scivi-client")
#define APPLICATION_VERSION QStringLiteral("2.0.0")

namespace scivi {

class SciViApplication: public QObject
{
public:
    SciViApplication(QQmlApplicationEngine &engine);
    void initialize(Settings &settings, QApplication &app, QQmlApplicationEngine &engine);
    int run(QApplication &app);

private:
    QSharedPointer<Editor> m_editor;
    QSharedPointer<PluginManager> m_pluginManager;
};

}

#endif // SCIVIAPPLICATION_H
