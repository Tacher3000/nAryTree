#include "scalablegraphicsview.h"

ScalableGraphicsView::ScalableGraphicsView(QWidget *parent) : QGraphicsView(parent), m_isDragging(false) {}

void ScalableGraphicsView::wheelEvent(QWheelEvent *event){
    QPoint angle = event->angleDelta();
    if (!angle.isNull()) {
        qreal scaleFactor = (angle.y() > 0) ? SCALE_FACTOR : INVERSE_SCALE_FACTOR;
        scale(scaleFactor, scaleFactor);
    }
    event->accept();
}

void ScalableGraphicsView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    event->accept();
}

void ScalableGraphicsView::mouseMoveEvent(QMouseEvent *event){
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;
        if (horizontalScrollBar() && verticalScrollBar()) {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        }
        m_lastMousePos = event->pos();
    }
    event->accept();
}

void ScalableGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
    event->accept();
}
