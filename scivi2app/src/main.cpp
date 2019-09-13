#include "precompiled_header.h"
#include <QApplication>
#include <QQmlApplicationEngine>

#include "sciviapplication.h"
#include "statusbarlogger.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication qtApp(argc, argv);
    qInstallMessageHandler(StatusBarLogger::messageHandler);
    QQmlApplicationEngine qmlEngine;

    scivi::Settings settings(qtApp);
    scivi::SciViApplication sciviApp(qmlEngine);
    sciviApp.initialize(settings, qtApp, qmlEngine);

    return sciviApp.run(qtApp);
}
