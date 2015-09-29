#pragma once

#include <QtWidgets/QtWidgets>
#include <Qt>
#include <memory>

#include "VertexContext.h"
#include "Vertex.h"
#include "Config.h"
#include "Labels.h"
#include "Typedefs.h"

class VertexImage : public QGraphicsItem
{
	VertexPtr _vertex;
	VertexContext const * _context;
	QPointF offset;

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
	inline VertexPtr getVertex() const { return _vertex; }
	void setVertex(VertexPtr const & val) { _vertex = val; }
};
