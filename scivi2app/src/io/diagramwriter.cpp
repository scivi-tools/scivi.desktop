#include "diagramwriter.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include "node.h"
#include "edge.h"
#include "datanode.h"
#include "dataflowdiagram.h"
#include "editor/nodefactory.h"
#include <utils/path.h>
#include "knowledge/knowledgeservice.h"


namespace scivi {
namespace diagram {

SharedDiagram readFromFile(const QString &filepath, QQmlEngine *engine)
{
    auto fixedPath = PathUtils::removeSchema(filepath);
    QFile file(fixedPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file with diagram at " << fixedPath;
        return nullptr;
    }
    QByteArray content = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(content);
    QJsonObject rootObject = doc.object();
    auto ontFilename = rootObject["onto"].toString();
    auto diagram = QSharedPointer<DataflowDiagram>::create(ontFilename);
    scivi::knowledge::KnowledgeService knowledgeService(ontFilename);
    TypesStore typesStore(&knowledgeService, engine);
    QJsonArray nodesArray = rootObject["nodes"].toArray();
    QJsonArray linksArray = rootObject["links"].toArray();
    QList<QSharedPointer<Node>> nodes;
    foreach(const auto &jsonNode, nodesArray) {
        int nodeConceptId = jsonNode["id"].toString().toInt();
        int nodeX = jsonNode["x"].toInt();
        int nodeY = jsonNode["y"].toInt();
        auto node = scivi::diagram::createNode(nodeConceptId, knowledgeService, typesStore);
        QJsonObject settingsObject = jsonNode["settings"].toObject();
        foreach (const auto &settingsKey, settingsObject.keys()) {
            auto id = settingsKey.toInt();
            QVariant value = settingsObject[QString::number(id)].toVariant();
            node->changeSetting(id, value);
        }
        node->setX(nodeX);
        node->setY(nodeY);
        nodes << node;
    }
    QList<QSharedPointer<Edge>> edges;
    foreach(auto jsonEdge, linksArray) {
        int srcNodeIndex = jsonEdge["node_from"].toInt();
        int srcSocketId = jsonEdge["socket_from"].toString().toInt();
        int destNodeIndex = jsonEdge["node_to"].toInt();
        int destSocketId = jsonEdge["socket_to"].toString().toInt();
        QSharedPointer<Node> srcNode = nodes[srcNodeIndex];
        QSharedPointer<Node> destNode = nodes[destNodeIndex];
        auto edge = QSharedPointer<Edge>::create(srcNode->output(srcSocketId),
                                                 destNode->input(destSocketId));
        if (!jsonEdge["number"].isUndefined()) {
            edge.data()->setNumber(jsonEdge["number"].toInt());
        }
        edges << edge;
    }
    for (auto &node: nodes) {
        diagram->insertNode(node);
    }
    for (auto &edge: edges) {
        diagram->insertEdge(edge);
    }
    return diagram;
}

bool writeToFile(QJsonObject &obj, QString &filename) {
    QJsonDocument doc(obj);
    return writeToFile(doc, filename);
}

bool writeToFile(QJsonDocument &doc, QString &filename)
{
    filename = PathUtils::removeSchema(filename);
#ifdef Q_OS_ANDROID
    filename = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation)[0] + "/" + filename;
#endif
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open export file" << file.errorString();
        return false;
    }
    file.write(doc.toJson());
    return true;
}

}
}
