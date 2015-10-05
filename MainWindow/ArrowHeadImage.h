#pragma once

#include <QGraphicsItem>
#include <math.h>

class EdgeImage;

class ArrowHeadImage : public QGraphicsItem
{
	int _width;
	int _height;
	bool _filled;
	QPainterPath head;
	QPointF _center;
	QColor _color;

public:
	ArrowHeadImage(int width, int height, float theta, bool filled);
	~ArrowHeadImage();

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

	void updateCenterPoint();
	void setWidth(int width);
	void setHeight(int height);
	void setColor(QColor const & color);

	inline QPointF Center() const
	{
		return _center;
	}

	void updateShape();
};

