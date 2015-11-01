#pragma once

#include <QLineF>
#include "EdgeImage.h"

class VertexImage;
class TextItem;

class StraightEdgeImage : public EdgeImage
{
	QLineF _line;

public:
	StraightEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context);
	~StraightEdgeImage();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;

private:
	void setCenterPoint() override;
	void updateContextMenu(QList<QAction*> const & actionList) override;
	void updateTextItemPos();
};