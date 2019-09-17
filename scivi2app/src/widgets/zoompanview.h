#pragma once

#include <QPointF>
#include <QQuickItem>

namespace scivi {
namespace widget {

const auto zoomDiff = 0.05f;
const auto zoomMin = 0.1f;

class ZoomPanView : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QQuickItem *innerView READ innerView NOTIFY innerViewChanged)
    Q_PROPERTY(bool panEnabled READ panEnabled WRITE setPanEnabled NOTIFY
                   panEnableChanged)

public:
    explicit ZoomPanView(QQuickItem *parent = nullptr);
    QQuickItem *innerView() const;
    bool panEnabled() const;
    void setPanEnabled(bool panEnabled);

signals:
    void innerViewChanged();
    void panEnableChanged();
    void clicked(QPointF);

private:
    QQuickItem *m_innerView;
    QPointF m_lastPanPosition;
    bool m_panEnabled = true;
    bool m_isPanning;

    // QQuickItem interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
};

}  // namespace widget
}  // namespace scivi
