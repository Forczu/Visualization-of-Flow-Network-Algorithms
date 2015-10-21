#pragma once

#include <QPainterPath>
#include "EdgeImage.h"

class EdgeContext;
class VertexImage;

class LoopEdgeImage : public EdgeImage
{
	QPainterPath _path;
	std::vector<QPointF> _points;

public:
	LoopEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context);
	~LoopEdgeImage();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;
};

