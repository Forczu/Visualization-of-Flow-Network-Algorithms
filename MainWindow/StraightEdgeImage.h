#pragma once

#include <QLineF>
#include "EdgeImage.h"

class StraightEdgeImage : public EdgeImage
{
	QLineF _line;

public:
	StraightEdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context);
	~StraightEdgeImage();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;
};