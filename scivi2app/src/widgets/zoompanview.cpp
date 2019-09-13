#include "./zoompanview.h"

#include <QPinchGesture>
#include <QGestureRecognizer>
#include <QTouchEvent>

scivi::widget::ZoomPanView::ZoomPanView(QQuickItem *parent): QQuickItem(parent)
{
    m_innerView = new QQuickItem(this);
    m_innerView->setTransformOrigin(TransformOrigin::TopLeft);
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    emit innerViewChanged();
}

QQuickItem *scivi::widget::ZoomPanView::innerView() const
{
    return m_innerView;
}

bool scivi::widget::ZoomPanView::panEnabled() const
{
    return m_panEnabled;
}

void scivi::widget::ZoomPanView::setPanEnabled(bool panEnabled)
{
    m_panEnabled = panEnabled;
    emit this->panEnableChanged();
}

void scivi::widget::ZoomPanView::mousePressEvent(QMouseEvent *event)
{
    m_lastPanPosition = event->localPos();
    m_isPanning = false;
}

void scivi::widget::ZoomPanView::mouseMoveEvent(QMouseEvent *event)
{
    auto delta = m_lastPanPosition - event->localPos();
    m_innerView->setPosition(m_innerView->position() - delta);
    m_isPanning = true;
    m_lastPanPosition = event->localPos();
}

void scivi::widget::ZoomPanView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (!m_isPanning) {
        emit this->clicked(event->localPos());
    }
    m_isPanning = false;
}

void scivi::widget::ZoomPanView::wheelEvent(QWheelEvent *event)
{
    m_isPanning = false;
    qreal oldScale = innerView()->scale();
    qreal newScale = oldScale + (event->angleDelta().y() > 0 ? zoomDiff : -zoomDiff);
    if (newScale < zoomMin)
        return;
    QPointF wheelTargetPoint(static_cast<qreal>(event->x()), static_cast<qreal>(event->y()));
    QPointF innerPoint = wheelTargetPoint - m_innerView->position();
    QPointF correction(innerPoint  - innerPoint * (newScale / oldScale));
    m_innerView->setPosition(m_innerView->position() + correction);
    innerView()->setScale(newScale);
}
