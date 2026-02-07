#include "Canvas.h"
#include <iostream>

void Canvas::mousePressEvent(QMouseEvent* event)
{
	std::cout << 1 << std::endl;
	if(event->button() == Qt::LeftButton) {
		path.moveTo(event->pos());
		path.lineTo(event->pos());
		this->update();
	}	
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
	path.lineTo(event->pos());
	this->update();
}

void Canvas::mouseReleaseEvent(QMouseEvent* event)
{
	path.lineTo(event->pos());
	this->update();
}

void Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen pen(Qt::black, pensize);
	painter.setPen(pen);
	painter.drawPath(path);
	painter.end();
}

void Canvas::saveImage()
{
	grab().save(QString("Images/%1image%2.png").arg(9).arg(counter++));
	path = QPainterPath();
	this->update();
}
