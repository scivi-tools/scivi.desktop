#ifndef NODEVIEW_H
#define NODEVIEW_H

#include <QPointer>
#include <QQuickItem>
#include "./node.h"

namespace scivi {
namespace diagram {

QPointF calcSocketCenter(QQuickItem *socketView);

class NodeView : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(Node *node READ node CONSTANT FINAL)
    Q_PROPERTY(bool isSelected READ isSelected WRITE setSelected NOTIFY
                   selectedChanged)
public:
    explicit NodeView(QQuickItem *parent = nullptr);
    Node *node();
    void setNode(Node *node);
    QQuickItem *getInputSocket(int index);
    QQuickItem *getOutputSocket(int index);
    bool isSelected() const;
    void setSelected(bool isSelected);

    Q_INVOKABLE void inputCreated(QQuickItem *socketView);
    Q_INVOKABLE void outputCreated(QQuickItem *socketView);

signals:
    void bodyClicked(NodeView *view, int modifiers);
    void inputClicked(NodeView *view, QQuickItem *socketView,
                      NodeSocket *socket);
    void outputClicked(NodeView *view, QQuickItem *socketView,
                       NodeSocket *socket);
    void inputEntered(QQuickItem *socketView);
    void inputExited(QQuickItem *socketView);
    void outputEntered(QQuickItem *socketView);
    void outputExited(QQuickItem *socketView);
    void deleteClicked(NodeView *view);
    void selectedChanged(bool);

private:
    QPointer<Node> m_node{nullptr};
    bool m_isSelected = false;
    QList<QPointer<QQuickItem>> m_inputSockets;
    QList<QPointer<QQuickItem>> m_outputSockets;
};

}  // namespace diagram
}  // namespace scivi

QML_DECLARE_TYPE(scivi::diagram::NodeView)

#endif  // NODEVIEW_H
