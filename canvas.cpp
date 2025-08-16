#include "canvas.h"
#include <QDebug>
#include <QMouseEvent>
#include <QTransform>

Canvas::Canvas() {}

void Canvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    QTransform transform;
    transform.scale(zoom,zoom);
    for(const QPainterPath &path : paths){
        QPainterPath tpath = transform.map(path);
        painter.drawPath(tpath);
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
        zoom += 0.15;
    else
        zoom -= 0.15;
    update();
}
