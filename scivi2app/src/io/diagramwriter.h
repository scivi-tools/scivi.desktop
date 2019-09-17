#ifndef DIAGRAMWRITER_H
#define DIAGRAMWRITER_H

#include <QPair>
#include <QQmlEngine>
#include <QSharedPointer>
#include <QString>

#include <dataflowdiagram.h>

namespace scivi {

namespace diagram {

using SharedDiagram = QSharedPointer<DataflowDiagram>;

bool writeToFile(QJsonObject &obj, QString &filename);
bool writeToFile(QJsonDocument &doc, QString &filename);
SharedDiagram readFromFile(const QString &filepath, QQmlEngine *engine);

}  // namespace diagram
}  // namespace scivi
#endif  // DIAGRAMWRITER_H
