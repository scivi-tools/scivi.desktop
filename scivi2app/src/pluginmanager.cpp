#include "pluginmanager.h"

#include <QDirIterator>
#include <QObject>
#include <QPluginLoader>
#include <QQmlContext>
#include <QQuickItem>

#include <plugin.h>

namespace scivi {

PluginManager::PluginManager(Editor *editor, QQmlEngine *engine,
                             QObject *parent)
    : QObject(parent), m_editor(editor), m_engine(engine) {
    m_editor = editor;
    m_engine = engine;
}

void PluginManager::initialize(QDir baseDir) {
    const auto plugins = findPluginsInDir(baseDir);
    for (const auto &plugin : plugins) {
        registerPlugin(plugin, baseDir.path());
    }
}

void PluginManager::registerPlugin(Plugin *plugin, QString dir) {
    qDebug() << "Register plugin " << plugin->name() << "at " << dir;
    plugin->initialize({});
    m_pluginModel.addPlugin(plugin);
}

Plugin *PluginManager::load(QString pathToPlugin) {
    QPluginLoader loader(pathToPlugin);
    QObject *instance = loader.instance();
    if (!instance) {
        qWarning() << "Couldn't load plugin: " << pathToPlugin
                   << ". Error: " << loader.errorString();
    }
    Plugin *plugin = qobject_cast<Plugin *>(instance);
    return plugin;
}

QList<Plugin *> PluginManager::plugins() const {
    return m_pluginModel.plugins();
}

QObject *PluginManager::pluginModel() {
    return reinterpret_cast<QObject *>(&m_pluginModel);
}

QQuickItem *PluginManager::runPluginWithName(QString name) {
    qDebug() << "Running plugin " << name;
    Plugin *plugin = m_pluginModel.findByName(name);
    DataflowDiagram *diagram = m_editor->diagram();
    const Plugin::StartupParams startupParams{
        diagram,
    };
    plugin->startup(startupParams);
    const auto obj = plugin->createView(m_engine);
    QQmlContext *context = new QQmlContext(m_engine, this);
    context->setContextProperty("diagram", qVariantFromValue(diagram));
    QQuickItem *item = qobject_cast<QQuickItem *>(obj->create(context));
    if (!item) {
        qWarning() << "Plugin root view is not QQuickItem";
    }
    return item;
}

void PluginManager::setPluginContainer(QQuickItem *container) {
    m_pluginContainer = container;
}

QList<Plugin *> PluginManager::findPluginsInDir(QDir dir) {
    qDebug() << "Loading plugins from " << dir.path();
    QList<Plugin *> plugins;
    for (const auto &pluginDirName :
         dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir pluginDir(dir.absoluteFilePath(pluginDirName));
        for (const auto &fileName : pluginDir.entryList(QDir::Files)) {
            QString path = pluginDir.absoluteFilePath(fileName);
            if (!QLibrary::isLibrary(path)) {
                continue;
            }
            const auto plugin = PluginManager::load(path);
            if (plugin) {
                plugins.append(plugin);
            }
        }
    }
    return plugins;
}

}  // namespace scivi
