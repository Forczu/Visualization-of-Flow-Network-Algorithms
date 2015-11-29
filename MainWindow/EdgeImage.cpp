#include "EdgeImage.h"
#include "VertexImage.h"
#include "ArrowHeadImage.h"
#include "EdgeContext.h"
#include "Edge.h"
#include "TextItem.h"
#include "ArrowHeadImage.h"
#include "GraphImage.h"
#include "MyMath.h"

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
	updateText();
}

int EdgeImage::getFlow() const
{
	return getEdge()->getFlow();
}

int EdgeImage::getCapacity() const
{
	return getEdge()->getCapacity();
}

void EdgeImage::changeFlow(int flow)
{
	setFlow(flow);
	if (_text != nullptr)
		_text->updateText();
}

void EdgeImage::setFlow(int flow)
{
	_edge->setFlow(flow);
	if (_text != nullptr)
		updateText();
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
	if (_text != nullptr)
		updateText();
}

void EdgeImage::setValues(int flow, int capacity)
{
	_edge->setFlow(flow);
	_edge->setCapacity(capacity);
	if (_text != nullptr)
		updateText();
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
	checkNewLine();
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

void EdgeImage::updateArrowHead(float angle)
{
	_arrow->setRotation(angle);
	_arrow->setPos(_vertexTo->PointAt(getEdge()->Id()) - pos());
	_arrow->updateCenterPoint();
}

void EdgeImage::createArrowHead()
{
	if (_arrow != nullptr)
		return;
	_arrow = new ArrowHeadImage(this, 50, 70, 0.0f, true);
	_arrow->setZValue(ARROWHEAD_Z_VALUE);
	_arrow->setParentItem(this);
	float angle = -getAngle() - 90;
	updateArrowHead(angle);
}

void EdgeImage::checkNewLine()
{
	setPos(_vertexFrom->PointAt(getEdge()->Id()));
	QLineF newLine = QLineF(_vertexFrom->pos(), _vertexTo->pos());
	calculateNewLine(newLine);
}

void EdgeImage::calculateNewLine(QLineF const & newLine)
{
	float fromAngle = newLine.angle();
	float toAngle = std::fmod(fromAngle + MyMath::HALF_FULL_ANGLE, MyMath::FULL_ANGLE);
	if (_offset.first)
	{
		fromAngle = MyMath::wrapAngle(fromAngle - _offset.second);
		toAngle = MyMath::wrapAngle(toAngle + _offset.second);
	}
	_vertexFrom->setPointForEdge(_edge->Id(), fromAngle);
	_vertexTo->setPointForEdge(_edge->Id(), toAngle);
	_actualLine = newLine;
}

void EdgeImage::updateText()
{
	QString capacity = QString::number(_edge->getCapacity());
	int flow;
	if ((flow = _edge->getFlow()) != 0)
		_text->setText(QString::number(flow)+ " / " + capacity);
	else
		_text->setText(capacity);
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
