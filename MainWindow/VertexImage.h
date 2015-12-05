#pragma once

#include <QtWidgets/QtWidgets>
#include "VertexContext.h"
#include "Vertex.h"
#include "Typedefs.h"
#include "GraphImage.h"

class EdgeImage;

class VertexImage : public QGraphicsItem
{
	GraphImage * parent;
	Vertex * _vertex;
	VertexContext * _context;
	PointMap _pointList;

	int _inDegree;
	int _outDegree;
public:
	explicit VertexImage(VertexContext * context);
	~VertexImage();

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	void setLabel(QString const & text);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	void drawVertex(QPainter *painter);
	void drawNumber(QPainter *painter);
public:
	void Context(VertexContext * val) { _context = val; }
	inline VertexContext const * Context() const { return _context; }
	inline Vertex * getVertex() const { return _vertex; }
	void setVertex(Vertex * const val) { _vertex = val; }
	void addEdgePoint(EdgeImage * edge, VertexImage * vertex, bool first);

	inline QPointF PointAt(int edgeId) { return _pointList[edgeId]; }
	inline PointMap getPoints() const { return _pointList; }
	void setPoints(PointMap const & points) { _pointList = points; };

	void setPointForEdge(int edgeId, float angle);
	void removePointForEdge(int edgeId);
	void setToolTip(int degree);
	void setToolTip(int indegree, int outdegree);

	inline GraphImage * getParent() const { return parent; }
	void setParent(GraphImage * val) { parent = val; }

	int getId() const;

	int getInDegree() const { return _inDegree; }
	void setInDegree(int val) { _inDegree = val; }
	int getOutDegree() const { return _outDegree; }
	void setOutDegree(int val) { _outDegree = val; }

	void setContext(VertexContext * context) { _context = context; }

private:
	QPointF findPointOnCircle(float angle);
};
