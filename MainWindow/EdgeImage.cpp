#include "EdgeImage.h"
#include "VertexImage.h"
#include "ArrowHeadImage.h"
#include "EdgeContext.h"
#include "Edge.h"
#include "TextItem.h"
#include "ArrowHeadImage.h"
#include "GraphImage.h"

EdgeImage::EdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context)
: _edge(edge), _vertexFrom(vertexFrom), _vertexTo(vertexTo), _context(context), _arrow(nullptr)
{
	_actualLine = QLineF(_vertexFrom->pos(), _vertexTo->pos());
	vertexFrom->addEdgePoint(this, vertexTo, true);
	vertexTo->addEdgePoint(this, vertexFrom, false);
	_text = new TextItem(pos().x(), pos().y());
	_text->replaceFont(QFont("Calibri", 30, 0, false));
	_text->setParentItem(this);
	_text->setRegex("[[:digit:]]+");	// tylko liczby nieujemne
	connect(_text, SIGNAL(valueChanged(QString const &)), this, SLOT(changeText(QString const &)));
	_offset.first = false;
	_offset.second = 0.0f;
}

EdgeImage::~EdgeImage()
{
	deleteArrowHead();
}

float EdgeImage::Angle() const
{
	return _actualLine.angle();
}

void EdgeImage::correctEdge(bool type, float theta)
{
	_offset.first = type;
	_offset.second = theta;
}

void EdgeImage::deleteArrowHead()
{
	delete _arrow;
	_arrow = nullptr;
}

void EdgeImage::setWeight(int weight)
{
	_text->setText(QString::number(weight));
	_edge->setWeight(weight);
}

void EdgeImage::addArrowHead()
{
	if (_arrow != nullptr)
		return;
	float angle = -Angle() - 90;
	_arrow = new ArrowHeadImage(this, 50, 70, angle, true);
	_arrow->setPos(_vertexTo->PointAt(getEdge()->Id()));
	_arrow->setZValue(ARROWHEAD_Z_VALUE);
	scene()->addItem(_arrow);
}

void EdgeImage::calculateNewLine()
{
	QLineF newLine = QLineF(_vertexFrom->pos(), _vertexTo->pos());
	float angle = newLine.angleTo(_actualLine);
	if (angle < 10.0f)
		return;
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
	_edge->setWeight(value);
}
