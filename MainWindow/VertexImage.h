#pragma once

#include <QtWidgets/QtWidgets>
#include <Qt>
#include <memory>

#include "VertexContext.h"
#include "Vertex.h"
#include "Config.h"
#include "Labels.h"
#include "Typedefs.h"

class EdgeImage;

class VertexImage : public QGraphicsItem
{
	Vertex * _vertex;
	VertexContext const * _context;
	QPointF offset;

	QMap<int, QPointF> _pointList;

public:
	VertexImage(VertexContext const & context);
	~VertexImage();

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

	void setLabel(QString const & text);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

public:
	void Context(VertexContext const & val) { _context = &val; }
	inline VertexContext const * Context() const { return _context; }
	inline Vertex * getVertex() const { return _vertex; }
	void setVertex(Vertex * const val) { _vertex = val; }

	void addEdgePoint(EdgeImage * edge, VertexImage * vertex, bool first);

	inline QPointF PointAt(int edgeId) const
	{
		return _pointList[edgeId];
	}

	void setPointForEdge(int edgeId, float angle);
	void setToolTip(int degree);
	void setToolTip(int indegree, int outdegree);
	
private:
	QPointF findPointOnCircle(float angle);
};
