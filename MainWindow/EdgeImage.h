#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPainter>
#include <vector>
#include "Typedefs.h"
#include "EdgeTextItem.h"
#include <QAction>

class ArrowHeadImage;
class Edge;
class EdgeContext;
class GraphImage;
class TextItem;
class VertexImage;

class EdgeImage : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
protected:
	Edge * _edge;
	EdgeContext * _context;
	QPointF _center;
	VertexImage * _vertexFrom;
	VertexImage * _vertexTo;
	ArrowHeadImage * _arrow;
	GraphImage * _parent;
	QLineF _actualLine;
	EdgeTextItem * _text;

	std::pair<bool, float> _offset;
	bool _hasNeighbor;
public:
	explicit EdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context);
	virtual ~EdgeImage();

	inline Edge * getEdge() const { return _edge; }
	void setEdge(Edge * const val) { _edge = val; }
	inline VertexImage * VertexFrom() const { return _vertexFrom; }
	inline VertexImage * VertexTo() const { return _vertexTo; }
	inline EdgeContext * Context() const { return _context; }
	void Context(EdgeContext * val) { _context = val; }
	inline QLineF ActualLine() const { return _actualLine; }
	void ActualLine(QLineF const & val) { _actualLine = val; }
	inline ArrowHeadImage * getArrowHead() const { return _arrow; }
	inline std::pair<bool, float> getOffset() const { return _offset; }
	inline EdgeTextItem * getTextItem() const { return _text; }
	void setTextItem(EdgeTextItem * text);
	inline bool hasNeighbor() const { return _hasNeighbor; }
	void setNeighbor(bool val) { _hasNeighbor = val; }

	void setOffset(bool b, float theta = 0.0f) { _offset = std::make_pair(b, theta); }
	inline QPointF center() const { return _center; }

	int getFlow() const;
	int getCapacity() const;
	void setParent(GraphImage * graph) { _parent = graph; }
	inline GraphImage * getParent() const { return _parent; }

	void changeFlow(int flow);
	void changeCapacity(int capacity);
	void setFlow(int flow);
	void setCapacity(int capacity);
	void setValues(int flow, int capacity);
	void scaleText(float scale);
	void setSelected(bool selected);

	qreal scaleText() const;
	float getAngle() const;
	void correctEdge(bool type, float theta);
	void createArrowHead();
	void deleteArrowHead();
	void setWeight(int weight);

	void checkNewLine();
protected:
	void calculateNewLine(QLineF const & newLine);
	void updateText();
	virtual void updateContextMenu(QList<QAction*> const & actionList) = 0;
	void updateArrowHead(float angle);
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
public:
	virtual void setCenterPoint() = 0;
	void setContext(EdgeContext * context) { _context = context; }
signals:
	void typeChanged(EdgeImage * edge, QAction * action);

private slots:
	void changeText(QString const & str);
};

