#pragma once

#include <QGraphicsItem>
#include "VertexImage.h"
#include "Edge.h"
#include <QPainterPath>
#include <QPainter>
#include <vector>

class EdgeImage : public QGraphicsItem
{
protected:
	Edge * edge;

	VertexImage * _vertexFrom;
	VertexImage * _vertexTo;

public:
	EdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo);
	~EdgeImage();

	inline Edge * getEdge() const { return edge; }
	void setEdge(Edge * val) { edge = val; }
};

