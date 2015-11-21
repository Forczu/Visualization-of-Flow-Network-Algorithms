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

	QLineF _actualLine;
	EdgeTextItem * _text;

	std::pair<bool, float> _offset;

public:
	EdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context);
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
	void setOffset(bool b, float theta) { _offset = std::make_pair(b, theta); }
	inline QPointF center() const { return _center; }

	void changeFlow(int flow);
	void setFlow(int flow);
	void changeCapacity(int capacity);
	void setCapacity(int capacity);
	void scaleText(float scale);
	void setSelected(bool selected);

	qreal scaleText() const;
	float getAngle() const;
	void correctEdge(bool type, float theta);
	void addArrowHead();
	void deleteArrowHead();
	void setWeight(int weight);

protected:
	void checkNewLine();
	void calculateNewLine(QLineF const & newLine);

	void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) Q_DECL_OVERRIDE;
	virtual void updateContextMenu(QList<QAction*> const & actionList) = 0;
public:
	virtual void setCenterPoint() = 0;
signals:
	void typeChanged(EdgeImage * edge, QAction * action);

private slots:
	void changeText(QString const & str);
};

