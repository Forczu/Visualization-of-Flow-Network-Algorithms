#pragma once

#include <QtWidgets/QtWidgets>
#include <Qt>

#include "VertexContext.h"
#include "Vertex.h"
#include "TextItem.h"
#include "Config.h"

class VertexImage : public QGraphicsItem
{
	Vertex * _vertex;

public:
	VertexImage(VertexContext const & context);
	~VertexImage();

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

	Vertex * getVertex() const { return _vertex; }
	void setVertex(Vertex * val) { _vertex = val; }

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:
	VertexContext _context;
	QPointF offset;

public:
	void Context(VertexContext const & val) { _context = val; }
	inline VertexContext const & Context() const { return _context; }
};
