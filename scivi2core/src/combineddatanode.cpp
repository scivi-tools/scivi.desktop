#include "combineddatanode.h"

#include "edge.h"
#include "edgeview.h"

namespace scivi {
namespace diagram {

CombinedDataNode::CombinedDataNode(QString name, QList<scivi::diagram::DataNode *> nodes, QObject *parent)
    : DataNode(), m_nodes(nodes)
{
    m_name = name;
    QList<SharedSetting> combinedSettings;
    QList<SharedNodeSocket> combinedInputs;
    QList<SharedNodeSocket> combinedOutputs;
    QSet<int> nodeIdSet;
    for (const auto &node: nodes) {
        nodeIdSet.insert(node->id());
    }
    for (const auto &node: nodes) {
        combinedSettings.append(node->settings());
        for (const auto &input: node->inputs()) {
            auto i = SharedNodeSocket::create(*input.data(), this);
            if (!input->isJoined()) {
                combinedInputs.append(i);
            } else {
                auto connectedEdges = input->joinedEdges();
                bool connectedToNodeSet = true;
                for (const auto &edge: connectedEdges) {
                    const auto srcId = edge->src()->node()->id();
                    if (!nodeIdSet.contains(srcId)) {
                        connectedToNodeSet = false;
                        m_inputsMapping.insert(i, input);
                        input->disjoinEdge(edge);
                        i->joinEdge(edge);
                        edge->setDest(i);
                    } else {
                        edge->view()->setVisible(false);
                    }
                }
                if (!connectedToNodeSet) {
                    combinedInputs.append(i);
                }
            }
        }
        for (const auto &output: node->outputs()) {
            auto o = SharedNodeSocket::create(*output.data(), this);
            if (!output->isJoined()) {
                combinedOutputs.append(o);
            } else {
                auto connectedEdges = output->joinedEdges();
                bool connectedToNodeSet = true;
                for (const auto &edge: connectedEdges) {
                    const auto destId = edge->dest()->node()->id();
                    if (!nodeIdSet.contains(destId)) {
                        connectedToNodeSet = false;
                        m_inputsMapping.insert(o, output);
                        output->disjoinEdge(edge);
                        o->joinEdge(edge);
                        edge->setSrc(o);
                    } else {
                        edge->view()->setVisible(false);
                    }
                }
                if (!connectedToNodeSet) {
                    combinedOutputs.append(o);
                }
            }
        }
    }
    setSettings(combinedSettings);
    setInputs(combinedInputs);
    setOutputs(combinedOutputs);
}

}
}
