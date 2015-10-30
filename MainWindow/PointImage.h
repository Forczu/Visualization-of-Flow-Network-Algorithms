#pragma once
#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include "Consts.h"

class PointImage : public QGraphicsItem
{
protected:
	static const int DIAMETER = 10;
	static const int STROKE = 2;

	QColor _inColor;
	
public:
	PointImage(QPointF const & position);
	~PointImage();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
};

