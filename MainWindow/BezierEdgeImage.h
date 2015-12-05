#pragma once
#include "PointImage.h"
#include "EdgeImage.h"
class VertexContext;

class BezierEdgeImage : public EdgeImage
{
	QPainterPath _path;
	QPainterPath _selection;
	std::vector<QPointF> _points;
	QLineF _line;
	PointImage * _p1;
	PointImage * _p2;
	bool _isMoveMode;

public:
	explicit BezierEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context);
	~BezierEdgeImage();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void setCenterPoint() override;
	void rotateArrowHead();
	float Angle(float px1, float py1, float px2, float py2);

	inline bool isMoveMode() const { return _isMoveMode; }
	void setMoveMode(bool moveMode) { _isMoveMode = moveMode; }
private:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	void updateContextMenu(QList<QAction*> const & actionList) override;
};

