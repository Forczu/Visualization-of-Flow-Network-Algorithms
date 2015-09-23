#pragma once

#include <QGraphicsItem>
#include "VertexImage.h"
#include "Edge.h"
#include <QPainterPath>
#include <QPainter>
#include <vector>

class EdgeImage : public QGraphicsItem
{
	Edge * edge;

	VertexImage * _vertexFrom;
	VertexImage * _vertexTo;

	std::vector<QPointF> _points;

public:
	EdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo);
	~EdgeImage();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;

	inline Edge * getEdge() const { return edge; }
	void setEdge(Edge * val) { edge = val; }
};

