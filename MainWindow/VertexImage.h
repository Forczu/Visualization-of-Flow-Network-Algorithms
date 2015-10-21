#pragma once

#include <QtWidgets/QtWidgets>
#include <Qt>
#include <memory>

#include "VertexContext.h"
#include "Vertex.h"
#include "Config.h"
#include "Labels.h"
#include "Typedefs.h"
#include "GraphImage.h"

class EdgeImage;

class VertexImage : public QGraphicsItem
{
	GraphImage * parent;
	Vertex * _vertex;
	VertexContext * _context;
	QPointF offset;

	QMap<int, QPointF> _pointList;

public:
	VertexImage(VertexContext * context);
	~VertexImage();

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

	void setLabel(QString const & text);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

public:
	void Context(VertexContext * val) { _context = val; }
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

	inline GraphImage * getParent() const { return parent; }
	void setParent(GraphImage * val) { parent = val; }
	
private:
	QPointF findPointOnCircle(float angle);
};
