#include "EdgeImage.h"
#include "VertexImage.h"
#include "ArrowHeadImage.h"
#include "EdgeContext.h"
#include "Edge.h"
#include "TextItem.h"
#include "ArrowHeadImage.h"
#include "GraphImage.h"

EdgeImage::EdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context)
: _edge(edge), _vertexFrom(vertexFrom), _vertexTo(vertexTo), _context(context), _arrow(nullptr), _text(nullptr)
{
	_actualLine = QLineF(_vertexFrom->pos(), _vertexTo->pos());
	vertexFrom->addEdgePoint(this, vertexTo, true);
	vertexTo->addEdgePoint(this, vertexFrom, false);
	_offset.first = false;
	_offset.second = 0.0f;
}

EdgeImage::~EdgeImage()
{
	deleteArrowHead();
}

void EdgeImage::setTextItem(EdgeTextItem * text)
{
	_text = text;
	_text->setText(QString::number(_edge->getCapacity()));
}

void EdgeImage::changeFlow(int flow)
{
	setFlow(flow);
	if (_text != nullptr)
		_text->updateText();
}

void EdgeImage::setFlow(int flow)
{
	if (_text != nullptr)
	{
		_edge->setFlow(flow);
		_text->setText(QString::number(flow) + " / " + QString::number(_edge->getCapacity()));
	}
}

void EdgeImage::changeCapacity(int capacity)
{
	setCapacity(capacity);
	if (_text != nullptr)
		_text->updateText();
}

void EdgeImage::setCapacity(int capacity)
{
	_edge->setCapacity(capacity);
}

void EdgeImage::scaleText(float scale)
{
	if (_text != nullptr)
		_text->setScale(scale);
}

qreal EdgeImage::scaleText() const
{
	if (_text != nullptr)
		return _text->scale();
	return 0.0f;
}

void EdgeImage::setSelected(bool selected)
{
	QGraphicsItem::setSelected(selected);
	if (_text != nullptr)
		_text->setSelected(selected);
}

float EdgeImage::getAngle() const
{
	return _actualLine.angle();
}

void EdgeImage::correctEdge(bool type, float theta)
{
	_offset.first = type;
	_offset.second = theta;
	calculateNewLine(QLineF(VertexFrom()->pos(), VertexTo()->pos()));
}

void EdgeImage::deleteArrowHead()
{
	delete _arrow;
	_arrow = nullptr;
}

void EdgeImage::setWeight(int weight)
{
	if (_text != nullptr)
		_text->setText(QString::number(weight));
	_edge->setCapacity(weight);
}

void EdgeImage::addArrowHead()
{
	if (_arrow != nullptr)
		return;
	float angle = -getAngle() - 90;
	_arrow = new ArrowHeadImage(this, 50, 70, angle, true);
	_arrow->setPos(_vertexTo->PointAt(getEdge()->Id()));
	_arrow->setZValue(ARROWHEAD_Z_VALUE);
	_arrow->setParentItem(this);
}

void EdgeImage::checkNewLine()
{
	QLineF newLine = QLineF(_vertexFrom->pos(), _vertexTo->pos());
	float angle = newLine.angleTo(_actualLine);
	if (angle < 10.0f)
		return;
	calculateNewLine(newLine);
}

void EdgeImage::calculateNewLine(QLineF const & newLine)
{
	float vFromAngle, vToAngle;
	vFromAngle = vToAngle = newLine.angle();
	if (_offset.first)
	{
		vFromAngle -= _offset.second;
		vToAngle += _offset.second;
	}
	_vertexFrom->setPointForEdge(_edge->Id(), vFromAngle);
	if (_vertexTo->pos().y() < _vertexFrom->pos().y())
	{
		vFromAngle += 180.0f;
		vToAngle += 180.0f;
	}
	else
	{
		vFromAngle -= 180.0f;
		vToAngle -= 180.0f;
	}
	_vertexTo->setPointForEdge(_edge->Id(), vToAngle);
	_actualLine = newLine;
}

void EdgeImage::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QString straight = "Linia prosta", bezier = "Linia krzywa";
	QMenu menu;
	QAction * changeIntoStraightAction = menu.addAction(straight);
	changeIntoStraightAction->setCheckable(true);
	QAction * changeIntoBezierAction = menu.addAction(bezier);
	changeIntoBezierAction->setCheckable(true);
	updateContextMenu(menu.actions());
	QAction * selectedAction = menu.exec(event->screenPos());
	EdgeType type;
	GraphImage * image = dynamic_cast<GraphImage*>(parent());
	if (NULL != image)
	{
		if (straight == selectedAction->text())
		{
			type = EdgeType::StraightLine;
		}
		else if (bezier == selectedAction->text())
		{
			type = EdgeType::BezierLine;
		}
		image->changeEdge(this, type);
	}
}

void EdgeImage::changeText(QString const & str)
{
	int value = str.toInt();
	_edge->setCapacity(value);
}
