#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QMap>
#include <QObject>
#include <QQuickItem>

#include <filterfactory.h>
#include <filterimplementation.h>

namespace scivi {

namespace diagram {

class DataflowDiagram;

class Interpreter : public QObject {
    Q_OBJECT
public:
    explicit Interpreter(QObject *parent = nullptr);
    ~Interpreter();
    void interpret(DataflowDiagram *diagram, QQuickItem *container);
signals:
    void interpretationStarted();

private:
    QMap<int, scivi::filters::FilterPtr> m_implementations;
};

}  // namespace diagram
}  // namespace scivi

#endif  // INTERPRETER_H
