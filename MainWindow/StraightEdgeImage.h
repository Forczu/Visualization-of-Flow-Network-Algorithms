#pragma once

#include <QLineF>
#include "EdgeImage.h"

class VertexImage;
class TextItem;

class StraightEdgeImage : public EdgeImage
{
	QLineF _line;
	QPointF _center;

public:
	StraightEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context);
	~StraightEdgeImage();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;

private:
	void updateCenterPoint();
	void updateTextItemPos();
};