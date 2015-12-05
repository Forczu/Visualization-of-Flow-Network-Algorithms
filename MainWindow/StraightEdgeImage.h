#pragma once

#include <QLineF>
#include "EdgeImage.h"
#include "MyMath.h"

class VertexImage;
class TextItem;

class StraightEdgeImage : public EdgeImage
{
	/// <summary>
	/// Linia, która definiuje liniê prost¹
	/// </summary>
	QLineF _line;

public:
	explicit StraightEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context);
	~StraightEdgeImage();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;
	QPainterPath shape() const Q_DECL_OVERRIDE;

private:
	void setCenterPoint() override;
	void updateContextMenu(QList<QAction*> const & actionList) override;
	void updateTextItemPos();
};