#include "sciviapplication.h"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QQmlContext>
#include <QQmlFileSelector>
#include <exception>

#include <edge.h>
#include <edgeview.h>
#include <graphview.h>
#include <nodesocket.h>
#include <nodeview.h>
#include <plugin.h>
#include <settingsmodel.h>
#include "settings.h"
#include "statusbarlogger.h"
#include "widgets/zoompanview.h"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>
#endif

namespace scivi {

SciViApplication::SciViApplication(QQmlApplicationEngine &engine)
    : m_editor(QSharedPointer<Editor>::create(&engine)),
      m_pluginManager(
          QSharedPointer<PluginManager>::create(m_editor.data(), &engine)) {}

void SciViApplication::initialize(Settings &settings, QApplication &app,
                                  QQmlApplicationEngine &engine) {
    app.setApplicationName(APPLICATION_NAME);
    app.setApplicationVersion(APPLICATION_VERSION);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.setApplicationDescription("SciVi Client");
    parser.addHelpOption();

    QCommandLineOption ontologyOpt("ont", tr("Read ontology from <file>."),
                                   "file");
    parser.addOption(ontologyOpt);
    parser.addPositionalArgument("diagram", tr("Diagram file to open"));
    settings.fromCommandLineArguments(parser, app.arguments());

    const QStringList args = parser.positionalArguments();

    QStringList selectors;

#ifdef QT_EXTRA_FILE_SELECTOR
    selectors += QT_EXTRA_FILE_SELECTOR;
#else
    if (app.arguments().contains("-touch")) selectors += "touch";
#endif
    QQmlFileSelector::get(&engine)->setExtraSelectors(selectors);

    if (parser.isSet(ontologyOpt)) {
        QString pathToOntology = parser.value(ontologyOpt);
        qDebug() << "Path to ontology " << pathToOntology;
        m_editor->setupFromFile(pathToOntology);
    }

    QString pathToDiagram = !args.isEmpty() ? args.at(0) : "";
    if (!pathToDiagram.isEmpty()) {
        qDebug() << "Path to diagram" << pathToDiagram;
        m_editor->importDiagramFrom(pathToDiagram);
    }

    m_pluginManager->initialize(settings.pluginsDir);

    engine.rootContext()->setContextProperty("logger",
                                             &StatusBarLogger::instance());
    engine.rootContext()->setContextProperty("applicationDirPath",
                                             app.applicationDirPath());
    engine.rootContext()->setContextProperty("editor", m_editor.data());
    engine.rootContext()->setContextProperty("pluginManager",
                                             m_pluginManager.data());

    qRegisterMetaType<scivi::diagram::Node *>("Node*");
    qRegisterMetaType<scivi::diagram::Edge *>("Edge*");
    qmlRegisterUncreatableType<scivi::diagram::NodeSocket>(
        "scivi", 0, 1, "NodeSocket", "cannot create nodesocket");
    qmlRegisterType<scivi::diagram::GraphView>("scivi", 0, 1, "GraphView");
    qmlRegisterType<scivi::diagram::NodeView>("scivi", 0, 1, "NodeView");
    qmlRegisterType<scivi::diagram::EdgeView>("scivi", 0, 1, "EdgeView");
    qmlRegisterType<scivi::SettingsModel>("scivi", 0, 1, "SettingsModel");
    qmlRegisterType<scivi::widget::ZoomPanView>("scivi", 0, 1, "ZoomPanView");

    QUrl appPath(QString("%1").arg(app.applicationDirPath()));
    engine.rootContext()->setContextProperty("appPath", appPath);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        throw std::runtime_error("qml root is empty");
    }
}

int SciViApplication::run(QApplication &app) {
#ifdef Q_OS_ANDROID
    QtAndroid::hideSplashScreen();
    QStringList perms;
    auto writeExternalStorage = "android.permission.WRITE_EXTERNAL_STORAGE";
    perms << writeExternalStorage;
    auto result = QtAndroid::checkPermission(writeExternalStorage);
    if (result == QtAndroid::PermissionResult::Denied) {
        auto resultHash = QtAndroid::requestPermissionsSync(perms);
        if (resultHash[writeExternalStorage] ==
            QtAndroid::PermissionResult::Denied) {
            return 0;
        }
    }
#endif

    return app.exec();
}

}  // namespace scivi
