#pragma once

#include <QtWidgets/QGraphicsView>
#include <QGraphicsScene>
#include "QTimeLine"
#include "QWheelEvent"
#include <QRubberBand>

#include "GraphScrollBar.h"
#include "Tools.h"
#include "VertexImage.h"
#include "EdgeImage.h"
#include "LoopEdgeImage.h"
#include "StraightEdgeImage.h"

class GraphView : public QGraphicsView
{
	Q_OBJECT

	static const int VERTICE_Z_VALUE = 3;
	static const int EDGE_Z_VALUE = 1;

	std::map<int, VertexImage*> _vertexMap;

	QRubberBand * _rubberBand;
	bool _rubberFlag;
	QPoint _origin;

private:
	bool _mouseClicked;

public:
	GraphView();
	GraphView(QWidget * widget);
	~GraphView();

	void addVertexImage(Vertex * const vertex, QPoint const & position);
	void addEdgeImage(Edge * const edge, std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord);
	void grabItem(QList<QGraphicsItem*> const & item);
	void startRubberBand(QPoint const & position);

protected:
	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

	QRect mapRubberBandToScene();

	void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent * event) Q_DECL_OVERRIDE;

private:
	void init();
	void unselectAll(QGraphicsItem * const except = nullptr);

signals:
	void clicked(QPoint, QList<QGraphicsItem*>);
	void moved(QPoint);

private:
	void changeSelection();
};

