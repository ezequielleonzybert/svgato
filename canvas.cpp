#include "canvas.h"

Canvas::Canvas() {}

void Canvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    for(const QPainterPath &path : paths){
        painter.drawPath(path);
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
