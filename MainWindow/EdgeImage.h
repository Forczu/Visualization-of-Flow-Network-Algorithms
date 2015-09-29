#pragma once

#include <QGraphicsItem>
#include "EdgeContext.h"
#include "VertexImage.h"
#include "Edge.h"
#include <QPainterPath>
#include <QPainter>
#include <vector>

class EdgeImage : public QGraphicsItem
{
protected:
	Edge * _edge;
	EdgeContext const * _context;

	VertexImage * _vertexFrom;
	VertexImage * _vertexTo;
public:
	EdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context);
	virtual ~EdgeImage();

	inline Edge * getEdge() const { return _edge; }
	void setEdge(Edge * val) { _edge = val; }
	inline VertexImage * VertexFrom() const { return _vertexFrom; }
	inline VertexImage * VertexTo() const { return _vertexTo; }
	inline EdgeContext const * Context() const { return _context; }
	void Context(EdgeContext const & val) { _context = &val; }
};

