#pragma once

#include <QtWidgets/QGraphicsView>
#include <QGraphicsScene>
#include <QTimeLine>
#include <QWheelEvent>
#include <QRubberBand>

#include "Orders.h"
#include "Weights.h"
#include "Tools.h"
#include "Typedefs.h"

class GraphScrollBar;
class VertexContext;
class VertexImage;
class EdgeContext;
class EdgeImage;
class LoopEdgeImage;
class StraightEdgeImage;
class TextItem;
class ArrowHeadImage;
class GraphImage;
class GraphScene;
class Tool;

enum class EdgeFlag
{
	Source, Target
};

class GraphView : public QGraphicsView
{
	Q_OBJECT

	static const float MIN_SCALE;
	static const float MAX_SCALE;
	static const float SCALE_FACTOR;

	TextItem * _sourceLabel;
	TextItem * _targetLabel;
	QRubberBand * _rubberBand;
	QPoint _origin;
	GraphImage * _graph;

private:
	bool _mouseClicked;
	bool _rubberFlag;
	bool _grabFlag;
	bool _sourceGlued, _targetGlued;
	EdgeFlag _edgeFlag;
	QPoint _offset;
	float _scale;

public:
	GraphView(GraphImage * graph);
	GraphView(Order order, Weight weighted);
	GraphView(QWidget * widget);
	~GraphView();

	void buildVertex(QPointF const & position, QList<QGraphicsItem*> const & items);
	void buildEdge(QGraphicsItem * const item);
	void remove(QList<QGraphicsItem*> const &items);

	void removeEdges(EdgeVector const & vector);

	void grabItem(QPointF const & pos);
	void pointItem(QPointF const & position, QList<QGraphicsItem*> const & item);
	void startRubberBand(QPointF const & position);
	void setTool(ToolType tool);

	EdgeFlag getEdgeFlag() const { return _edgeFlag; }
	void setEdgeFlag(EdgeFlag val) { _edgeFlag = val; }


	inline GraphImage * getGraphImage() const { return _graph; }
	void setGraphImage(GraphImage * val) { _graph = val; }
	inline float getScale() const { return _scale; }

protected:
	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

	QRect mapRubberBandToScene();
public:
	void changeVerticesLabels(QPoint const & position);

private:
	void init();
	void unselectAll(QGraphicsItem * const except = nullptr);

signals:
	void clicked(QPoint const & position, QList<QGraphicsItem*>);
	void moved(QPoint);
	void graphChanged();
	void scaleChanged(float);

private:
	void changeSelection();
	void createScene();
	void createGraph(Order order, Weight weighted);
	void glueLabel(EdgeFlag edgeFlag, VertexImage * img);
	void setSourceLabelPost(VertexImage * img);
	void setTargetLabelPos(VertexImage * img);
	void unglueLabels();
};

