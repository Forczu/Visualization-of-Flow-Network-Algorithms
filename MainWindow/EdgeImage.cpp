#include "EdgeImage.h"
#include "VertexImage.h"
#include "ArrowHeadImage.h"

EdgeImage::EdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context)
: _edge(edge), _vertexFrom(vertexFrom), _vertexTo(vertexTo), _context(&context), _arrow(nullptr)
{
	_actualLine = QLineF(_vertexFrom->pos(), _vertexTo->pos());
	vertexFrom->addEdgePoint(this, vertexTo, true);
	vertexTo->addEdgePoint(this, vertexFrom, false);
	_text = new TextItem();
	_text->setFlag(QGraphicsItem::ItemIsMovable);
	_text->setFlag(QGraphicsItem::ItemIsSelectable);
	_text->setPos(pos().x(), pos().y() - 75);
	_text->setText("12");
	_text->replaceFont(QFont("Calibri", 30, 0, false));
	_text->setParentItem(this);
	_offset.first = false;
	_offset.second = 0.0f;
}

EdgeImage::~EdgeImage()
{
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

void EdgeImage::removeArrowHead()
{
	delete _arrow;
	_arrow = nullptr;
}

void EdgeImage::addArrowHead()
{
	if (_arrow != nullptr)
		return;
	float angle = -Angle() - 90;
	_arrow = new ArrowHeadImage(50, 70, angle, true);
	_arrow->setPos(_vertexTo->PointAt(getEdge()->Id()));
	_arrow->setZValue(3);
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
