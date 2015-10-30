#include "PointImage.h"


PointImage::PointImage(QPointF const & position) : _inColor(QColor("yellow"))
{
	setPos(position);
	setFlags(ItemIsSelectable | ItemIsMovable);
	setZValue(POINT_Z_VALUE);
}


PointImage::~PointImage()
{
}

void PointImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setRenderHint(QPainter::RenderHint::HighQualityAntialiasing);
	QBrush brush;
	brush.setColor(_inColor);
	brush.setStyle(Qt::SolidPattern);
	painter->setBrush(brush);
	QPainterPath vertex;
	vertex.addEllipse(QPointF(0.0f, 0.0f), DIAMETER, DIAMETER);
	painter->setPen(QPen(Qt::black, STROKE, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter->drawPath(vertex);
}

QRectF PointImage::boundingRect() const
{
	int radius = DIAMETER / 2.0f;
	return QRectF(pos().x() - radius, pos().y() - radius, DIAMETER, DIAMETER);
}

QVariant PointImage::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSelectedChange)
	{
		if (value.toBool())
			_inColor = QColor("blue");
		else
			_inColor = QColor("yellow");
	}
	return QGraphicsItem::itemChange(change, value);
}

