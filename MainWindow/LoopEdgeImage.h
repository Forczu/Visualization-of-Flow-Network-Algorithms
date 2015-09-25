#pragma once

#include "EdgeImage.h"
#include <QPainterPath>

class LoopEdgeImage : public EdgeImage
{
	QPainterPath _path;
	std::vector<QPointF> _points;

public:
	LoopEdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo);
	~LoopEdgeImage();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;
};

