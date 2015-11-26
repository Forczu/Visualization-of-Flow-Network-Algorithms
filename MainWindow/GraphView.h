#pragma once

#include <QtWidgets/QGraphicsView>
#include <QGraphicsScene>
#include <QTimeLine>
#include <QWheelEvent>
#include <QRubberBand>

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
	QFont _labelFont;

private:
	bool _mouseClicked;
	bool _rubberFlag;
	bool _grabFlag;
	bool _sourceGlued, _targetGlued;
	EdgeFlag _edgeFlag;
	QPoint _offset;
	float _scale;
	bool _blocked;

public:
	GraphView(QWidget * parent = 0);
	GraphView(GraphImage * graph);
	~GraphView();

	void buildVertex(QPointF const & position, QList<QGraphicsItem*> const & items);
	void buildEdge(QGraphicsItem * const item);
	void remove(QList<QGraphicsItem*> const &items);

	void removeEdges(EdgeVector const & vector);

	void grabItem(QPointF const & pos);
	void startRubberBand(QPointF const & position);
	void setTool(ToolType tool);
	EdgeFlag getEdgeFlag() const { return _edgeFlag; }
	void setEdgeFlag(EdgeFlag val) { _edgeFlag = val; }


	inline GraphImage * getGraphImage() const { return _graph; }
	void setGraphImage(GraphImage * val);
	inline float getScale() const { return _scale; }
	void setBlocked(bool blocked) { _blocked = blocked; }

protected:
	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

	void alignItems(QList<QGraphicsItem*> const & chosenItems, int key);

	QRect mapRubberBandToScene();
public:
	void changeVerticesLabels(QPoint const & position);
	void addScene(QGraphicsScene * scene);
	void updateView();

private:
	void init();
	void createFont();
	void createLabel(TextItem *& label, QString const & text, Qt::AlignmentFlag align);
	void unselectAll(QGraphicsItem * const except = nullptr);

signals:
	void clicked(QPoint const & position, QList<QGraphicsItem*>);
	void moved(QPoint);
	void graphChanged();
	void scaleChanged(float);

private:
	void glueLabel(EdgeFlag edgeFlag, VertexImage * img);
	void setSourceLabelPost(VertexImage * img);
	void setTargetLabelPos(VertexImage * img);
	void unglueLabels();
	QList<QGraphicsItem*> takeGraphElements(QPoint const & position);
};

