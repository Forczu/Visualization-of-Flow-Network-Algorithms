#include "ArrowHeadImage.h"
#include "EdgeImage.h"

ArrowHeadImage::ArrowHeadImage(int width, int height, float theta, bool filled)
: _width(width), _height(height), _filled(filled)
{
	QPointF startPoint = pos();
	setTransformOriginPoint(startPoint);
	setRotation(theta);
	updateShape();
}

ArrowHeadImage::~ArrowHeadImage()
{
}

QRectF ArrowHeadImage::boundingRect() const
{
	return head.boundingRect();
}

void ArrowHeadImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->fillPath(head, _color);
	painter->drawPath(head);
}

void ArrowHeadImage::updateCenterPoint()
{
	QPointF startPoint = pos();
	float theta = (rotation() + 270) * M_PI / 180.0f;
	float height = _height / 2.0f;
	_center = QPointF(
		startPoint.x() + height * std::cos(theta),
		startPoint.y() + height * std::sin(theta));
}

void ArrowHeadImage::setWidth(int width)
{
	_width = width;
}

void ArrowHeadImage::setHeight(int height)
{
	_height = height;
}

void ArrowHeadImage::setColor(QColor const & color)
{
	_color = color;
}

void ArrowHeadImage::updateShape()
{
	QPointF startPoint = pos();
	QPointF rightButtom = QPointF(startPoint.x() + _width / 2.0f, startPoint.y() - _height);
	QPointF leftButtom = QPointF(startPoint.x() - _width / 2.0f, startPoint.y() - _height);
	head.moveTo(startPoint);
	head.lineTo(rightButtom);
	head.lineTo(leftButtom);
	head.lineTo(startPoint);
	updateCenterPoint();
}
