#ifndef COMBINEDDATANODE_H
#define COMBINEDDATANODE_H

#include "datanode.h"

namespace scivi {
namespace diagram {

class CombinedDataNode : public DataNode {
    Q_OBJECT
public:
    explicit CombinedDataNode(QString name, QList<DataNode *> nodes,
                              QObject *parent = nullptr);

private:
    QList<DataNode *> m_nodes;
    QMap<SharedNodeSocket, SharedNodeSocket> m_inputsMapping;
    QMap<SharedNodeSocket, SharedNodeSocket> m_outputsMapping;
};

}  // namespace diagram
}  // namespace scivi

#endif  // COMBINEDDATANODE_H
