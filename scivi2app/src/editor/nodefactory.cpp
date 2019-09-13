#include "nodefactory.h"

#include "settings.h"

QSharedPointer<scivi::diagram::DataNode> scivi::diagram::createNode(int conceptId, scivi::knowledge::KnowledgeService &service, TypesStore &typesStore)
{
    auto nodeConcept = service.conceptById(conceptId);
    auto nodeInstances = service.instancesOf(conceptId);
    const auto pathAttribute = "path";
    QString implementationPath = "";
    if (!nodeInstances.isEmpty()) {
        auto firstImplementation = nodeInstances.first();
        if (firstImplementation->attributes().contains(pathAttribute))
            implementationPath = firstImplementation->attributes()[pathAttribute].toString();
    }
    scivi::filters::FilterFactory* implFactory { nullptr };
    if (!implementationPath.isEmpty()) {
#ifdef Q_OS_ANDROID
        implementationPath = "lib" + implementationPath.replace("/", "_");
#endif
        implementationPath = getSettings()->filtersDir + "/" + implementationPath;
        QPluginLoader loader(implementationPath);
        QFileInfo checkFile(implementationPath);
        auto *plugin = loader.instance();
        if (plugin == nullptr) {
            qWarning() << "Cannot load plugin for node: " << nodeConcept->name() << " at " << implementationPath << "Error:" << loader.errorString();
            if (checkFile.exists()) {
                qWarning() << "but file exists";
            }
        } else {
            qDebug() << "Load implemtentation factory from" << implementationPath ;
            implFactory = qobject_cast<scivi::filters::FilterFactory*>(plugin);
        }
    } else {
        qWarning() << "Implementation path for node " << nodeConcept->name() << " is not set";
    }
    auto newNode = QSharedPointer<diagram::DataNode>::create(nodeConcept->name(), nodeConcept->id(), implFactory);
    auto outputConcept = service.conceptByName("Output");
    auto inputConcept = service.conceptByName("Input");
    auto nodeOutputConcepts = service.hasRelationsWithSubType(conceptId, outputConcept);
    auto nodeInputConcepts = service.hasRelationsWithSubType(conceptId, inputConcept);
    QList<SharedNodeSocket> inputSockets;
    QList<SharedNodeSocket> outputSockets;

    auto typeConcept = service.conceptByName("Type");
    for(auto &inputConcept : nodeInputConcepts) {
        auto inputTypeConcept = service.findMiddleConcept(inputConcept, typeConcept);
        QString colorAttribute = (*inputTypeConcept)["color"].toString();
        QColor color(colorAttribute);
        NodeSocketType nodeSocketType(inputTypeConcept->name(), color);
        inputSockets << SharedNodeSocket::create(inputConcept->id(), inputConcept->name(), NodeSocket::Type::In, nodeSocketType, newNode.data());
    }
    for(auto &outputConcept : nodeOutputConcepts) {
        auto outputTypeConcept = service.findMiddleConcept(outputConcept, typeConcept);
        QString colorAttribute = (*outputTypeConcept)["color"].toString();
        QColor color(colorAttribute);
        NodeSocketType nodeSocketType(outputTypeConcept->name(), color);
        outputSockets << SharedNodeSocket::create(outputConcept->id(), outputConcept->name(), NodeSocket::Type::Out, nodeSocketType, newNode.data());
    }
    newNode->setInputs(inputSockets);
    newNode->setOutputs(outputSockets);

    auto settingConcept = service.conceptByName("Setting");
    auto nodeSettingConcepts = service.hasRelationsWithSubType(conceptId, settingConcept);
    QList<QSharedPointer<Setting>> nodeSettingsList;
    for(auto setting : nodeSettingConcepts) {
        QVariant defaultValue = (*setting)["default"];
        auto settingTypeConcept = service.findMiddleConcept(setting, typeConcept);
        auto typeName = settingTypeConcept->name();
        auto settingComponent = typesStore.componentForType(typeName);
        nodeSettingsList.append(QSharedPointer<Setting>::create(setting->id(), setting->name(), defaultValue, settingComponent));
    }
    newNode->setSettings(nodeSettingsList);
    return newNode;
}
