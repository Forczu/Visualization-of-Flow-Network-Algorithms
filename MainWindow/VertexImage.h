#pragma once

#include <QtWidgets/QtWidgets>
#include <Qt>
#include <memory>

#include "VertexContext.h"
#include "Vertex.h"
#include "Config.h"
#include "Labels.h"

class VertexImage : public QGraphicsItem
{
	std::shared_ptr<Vertex> _vertex;
	EdgeLabel _edgeLabel;
	QString _label;

public:
	VertexImage(VertexContext const & context);
	~VertexImage();

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

	void setEdgeLabel(EdgeLabel val);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:
	VertexContext _context;
	QPointF offset;

public:
	void Context(VertexContext const & val) { _context = val; }
	inline VertexContext const & Context() const { return _context; }
	Vertex * getVertex() const { return _vertex.get(); }
	void setVertex(Vertex * val) { _vertex = std::shared_ptr<Vertex>(val); }
};
