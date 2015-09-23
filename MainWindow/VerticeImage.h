#pragma once

#include <QtWidgets/QtWidgets>
#include <Qt>

#include "VerticeContext.h"
#include "Vertice.h"
#include "TextItem.h"
#include "Config.h"

class VerticeImage : public QGraphicsItem
{
	Vertice * _vertice;
	TextItem * _text;

public:
	VerticeImage(VerticeContext const & context);

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

	Vertice * getVertice() const { return _vertice; }
	void setVertice(Vertice * val) { _vertice = val; }

	void select(bool selected);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
	VerticeContext _context;
	QPointF offset;

public:
	void Context(VerticeContext const & val) { _context = val; }
	inline VerticeContext const & Context() const { return _context; }
};
