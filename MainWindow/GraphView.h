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
#include "TextItem.h"
#include "ArrowHeadImage.h"

enum class EdgeFlag
{
	None, Source, Target
};

class GraphView : public QGraphicsView
{
	Q_OBJECT

	static const int ARROWHEAD_Z_VALUE = 3;
	static const int VERTICE_Z_VALUE = 2;
	static const int EDGE_Z_VALUE = 1;
	static const int EDGE_OFFSET = 30;

	typedef std::map<int, VertexImage*>					VertexImageMap;
	typedef std::map<std::pair<int, int>, EdgeImage*>	EdgeImageMap;
	typedef std::map<std::string, TextItem*>			LabelMap;

	VertexImageMap	_vertexMap;
	EdgeImageMap	_edgeMap;
	LabelMap _labelMap;

	QGraphicsScene * _graphScene;
	QRubberBand * _rubberBand;
	QPoint _origin;

private:
	bool _mouseClicked;
	bool _rubberFlag;
	bool _grabFlag;
	bool _addEdgeFlag;
	EdgeFlag _edgeFlag;
	Tool _toolFlag;
	QPoint _offset;

public:
	GraphView();
	GraphView(QWidget * widget);
	~GraphView();

	void addVertexImage(Vertex * const vertex, QPoint const & position);
	void addEdgeImage(Edge * const edge, std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord);

	void grabItem(QPoint const & pos);
	void pointItem(QPoint const & position, QList<QGraphicsItem*> const & item);
	void startRubberBand(QPoint const & position);
	void setTool(Tool tool);
	void removeEdge(EdgeImage * const edge);
	void removeVertex(VertexImage * const vertex);

	EdgeFlag getEdgeFlag() const { return _edgeFlag; }
	void setEdgeFlag(EdgeFlag val) { _edgeFlag = val; }
	void AddEdgeFlag(bool val) { _addEdgeFlag = val; }

	void correctNeighborEdges(Edge * const first, Edge * const second);

protected:
	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

	void changeVerticesLabels(QPoint const & position);

	QRect mapRubberBandToScene();

private:
	void init();
	void removeItem(QGraphicsItem * item);
	void unselectAll(QGraphicsItem * const except = nullptr);

signals:
	void clicked(QPoint const & position, QList<QGraphicsItem*>);
	void moved(QPoint);

private:
	void changeSelection();
	void AddArrowHeadToEdge(EdgeImage * edgeImg, VertexImage * vertexTo);
};

