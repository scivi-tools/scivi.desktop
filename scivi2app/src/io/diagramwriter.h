#ifndef DIAGRAMWRITER_H
#define DIAGRAMWRITER_H

#include <QString>
#include <QSharedPointer>
#include <QPair>
#include <QQmlEngine>

#include <dataflowdiagram.h>

namespace scivi {

namespace diagram {

using SharedDiagram = QSharedPointer<DataflowDiagram>;

bool writeToFile(QJsonObject &obj, QString &filename);
bool writeToFile(QJsonDocument &doc, QString &filename);
SharedDiagram readFromFile(const QString &filepath, QQmlEngine *engine);

}
}
#endif // DIAGRAMWRITER_H
