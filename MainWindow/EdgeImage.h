#pragma once

#include <QGraphicsItem>
#include "VerticeImage.h"
#include "Edge.h"
#include <QPainterPath>
#include <QPainter>
#include <vector>

class EdgeImage : public QGraphicsItem
{
	Edge * edge;

	VerticeImage * _verticeFrom;
	VerticeImage * _verticeTo;

	std::vector<QPointF> _points;

public:
	EdgeImage(VerticeImage * const verticeFrom, VerticeImage * const verticeTo);
	~EdgeImage();

	//void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	//QRectF boundingRect() const Q_DECL_OVERRIDE;

	inline Edge * getEdge() const { return edge; }
	void setEdge(Edge * val) { edge = val; }
};

