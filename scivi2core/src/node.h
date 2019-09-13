#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QObject>
#include <QQmlComponent>
#include <QQmlListProperty>

#include "./nodesocket.h"

namespace scivi {
namespace diagram {

class NodeView;

using SharedNodeSocket = QSharedPointer<NodeSocket>;

class Node : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Node)
    Q_PROPERTY(int nodeId READ id FINAL)
    Q_PROPERTY(float nodeX READ x WRITE setX)
    Q_PROPERTY(float nodeY READ y WRITE setY)
    Q_PROPERTY(QQmlListProperty<scivi::diagram::NodeSocket> inputs READ
                   inputsProp NOTIFY inputsChanged)
    Q_PROPERTY(QQmlListProperty<scivi::diagram::NodeSocket> outputs READ
                   outputsProp NOTIFY outputsChanged)
    Q_PROPERTY(NodeView *view READ view)

public:
    explicit Node(QObject *parent = nullptr);
    explicit Node(QList<SharedNodeSocket> inputs,
                  QList<SharedNodeSocket> outputs, QObject *parent = nullptr);
    ~Node();
    int id() const;
    float x() const;
    float y() const;
    QQmlListProperty<NodeSocket> inputsProp();
    QQmlListProperty<NodeSocket> outputsProp();
    virtual QList<SharedNodeSocket> inputs() const;
    virtual QList<SharedNodeSocket> outputs() const;
    NodeView *view();
    void setX(float x);
    void setY(float y);
    void setInputs(QList<SharedNodeSocket> inputs);
    void setOutputs(QList<SharedNodeSocket> outputs);
    void setView(NodeView *view);
    SharedNodeSocket input(int id);
    SharedNodeSocket output(int id);
    static QQmlComponent *delegate(QQmlEngine &engine);

    virtual QJsonObject toJsonObject() const;

signals:
    void idChanged();
    void inputsChanged();
    void outputsChanged();

protected:
    int m_id;
    float m_x;
    float m_y;
    QList<SharedNodeSocket> m_inputs;
    QList<SharedNodeSocket> m_outputs;
    NodeView *m_view;

private:
    int inputCount() const;
    NodeSocket *inputAt(int index) const;
    int outputCount() const;
    NodeSocket *outputAt(int index) const;

    static int inputCount(QQmlListProperty<NodeSocket> *property);
    static NodeSocket *inputAt(QQmlListProperty<NodeSocket> *property,
                               int index);
    static int outputCount(QQmlListProperty<NodeSocket> *property);
    static NodeSocket *outputAt(QQmlListProperty<NodeSocket> *property,
                                int index);

    static int lastGeneratedId;
};

}  // namespace diagram
}  // namespace scivi

QML_DECLARE_TYPE(scivi::diagram::Node)

#endif  // NODE_H
