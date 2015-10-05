#pragma once

#include <QGraphicsItem>
#include <QPainterPath>
#include <QPainter>
#include <vector>
#include "Typedefs.h"

class ArrowHeadImage;
class Edge;
class EdgeContext;
class TextItem;
class VertexImage;

class VertexImage;

class EdgeImage : public QGraphicsItem
{
protected:
	Edge * _edge;
	EdgeContext const * _context;

	VertexImage * _vertexFrom;
	VertexImage * _vertexTo;
	ArrowHeadImage * _arrow;

	QLineF _actualLine;
	TextItem * _text;

	std::pair<bool, float> _offset;

public:
	EdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context);
	virtual ~EdgeImage();

	inline Edge * getEdge() const { return _edge; }
	void setEdge(Edge * const val) { _edge = val; }
	inline VertexImage * VertexFrom() const { return _vertexFrom; }
	inline VertexImage * VertexTo() const { return _vertexTo; }
	inline EdgeContext const * Context() const { return _context; }
	void Context(EdgeContext const & val) { _context = &val; }
	inline QLineF ActualLine() const { return _actualLine; }
	void ActualLine(QLineF const & val) { _actualLine = val; }

	float Angle() const;
	void correctEdge(bool type, float theta);
	void addArrowHead();
	void deleteArrowHead();

protected:
	void calculateNewLine();
};

