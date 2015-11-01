#include "BezierEdgeImage.h"
#include "VertexImage.h"
#include "Edge.h"
#include "ArrowHeadImage.h"

BezierEdgeImage::BezierEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context)
: EdgeImage(edge, vertexFrom, vertexTo, context), _isMoveMode(false)
{
	QPointF start = _vertexFrom->PointAt(_edge->Id());
	QPointF end = _vertexTo->PointAt(_edge->Id());
	_line = QLineF(start, end);
	_points.push_back(start);
	if (_vertexFrom->getVertex()->Id() != _vertexTo->getVertex()->Id())
	{
		_points.push_back(_line.pointAt(0.33));
		_points.push_back(_line.pointAt(0.67));
	}
	else
	{
		int size = _context->Size();
		_points.push_back(QPointF(start.x() + 4 * size, start.y() - 6 * size));
		_points.push_back(QPointF(start.x() - 4 * size, start.y() - 6 * size));
	}
	_points.push_back(end);
	_p1 = new PointImage(_points[1]);
	_p2 = new PointImage(_points[2]);
	_p1->setParentItem(this);
	_p2->setParentItem(this);
	_p1->hide();
	_p2->hide();
	setFlags(ItemIsSelectable);
}

BezierEdgeImage::~BezierEdgeImage()
{
}

void BezierEdgeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	checkNewLine();
	QPointF oldCenter = _center;
	setCenterPoint();
	if (_center != oldCenter)
	{
		int dx = _center.x() - oldCenter.x();
		int dy = _center.y() - oldCenter.y();
		QPointF textPos = QPointF(
			_text->pos().x() + dx,
			_text->pos().y() + dy);
		_text->setPos(textPos);
	}
	if (_text->isSelected())
	{
		QLineF connection = QLineF(_center, _text->pos());
		painter->setPen(QPen(Qt::black, 2, Qt::DotLine));
		painter->drawLine(connection);
	}

	bool isArrow = _arrow != nullptr;
	QPointF arrowCenter;

	if (isArrow)
	{
		rotateArrowHead();
		_arrow->setPos(_vertexTo->PointAt(_edge->Id()));
		_arrow->updateCenterPoint();
		arrowCenter = _arrow->Center();
	}

	_points[0] = _vertexFrom->PointAt(_edge->Id());
	_points[3] = !isArrow ? _vertexTo->PointAt(_edge->Id()) : arrowCenter;
	_line.setPoints(_points[0], _points[3]);
	_points[1] = mapToScene(_p1->pos());
	_points[2] = mapToScene(_p2->pos());
	_path = QPainterPath();
	_path.moveTo(_points[0]);
	_path.cubicTo(_points[1], _points[2], _points[3]);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(_context->Color(), _context->Size(), Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	painter->drawPath(_path);
	if (isMoveMode())
	{
		_selection = QPainterPath();
		_selection.moveTo(_points[0]);
		_selection.lineTo(_points[1]);
		_selection.lineTo(_points[2]);
		_selection.lineTo(_points[3]);
		painter->setPen(QPen(Qt::black, 1, Qt::DashDotLine, Qt::RoundCap, Qt::MiterJoin));
		painter->drawPath(_selection);
	}
}

QRectF BezierEdgeImage::boundingRect() const
{
	return _path.boundingRect();
}

void BezierEdgeImage::setCenterPoint()
{
	_center = _path.pointAtPercent(0.5);
}

void BezierEdgeImage::rotateArrowHead()
{
	QPointF p0 = _points[0];
	QPointF p1 = _points[1];
	QPointF p2 = _points[2];
	QPointF p3 = _points[3];
	float x = 0, y = 0;
	float xold = 0, yold = 0;
	float angle = 0;
	for (float t = 0.90f; t < 1.001f; t += 0.01f)
	{
		x = std::pow(1 - t, 3) * p0.x() + 3 * std::pow(1 - t, 2) * t * p1.x() + 3 * (1 - t) * std::pow(t, 2) * p2.x() + std::pow(t, 3) * p3.x();
		y = std::pow(1 - t, 3) * p0.y() + 3 * std::pow(1 - t, 2) * t * p1.y() + 3 * (1 - t) * std::pow(t, 2) * p2.y() + std::pow(t, 3) * p3.y();
		angle = Angle(xold, yold, x, y);
		xold = x; yold = y;
	}
	_arrow->setRotation(angle - 90);
}

float BezierEdgeImage::Angle(float px1, float py1, float px2, float py2)
{
	// Negate X and Y values
	float pxRes = px2 - px1;
	float pyRes = py2 - py1;
	float angle = 0.0;
	// Calculate the angleror
	if (pxRes == 0.0)
	{
		if (pxRes == 0.0)
			angle = 0.0;
		else if (pyRes > 0.0) angle = M_PI / 2.0;
		else
			angle = M_PI * 3.0 / 2.0;
	}
	else if (pyRes == 0.0)
	{
		if (pxRes > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if (pxRes < 0.0)
			angle = std::atan(pyRes / pxRes) + M_PI;
		else if (pyRes < 0.0) angle = std::atan(pyRes / pxRes) + (2 * M_PI);
		else
			angle = std::atan(pyRes / pxRes);
	}
	// Convert to degrees
	angle = angle * 180 / M_PI;
	return angle;
}

QVariant BezierEdgeImage::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedChange)
	{
		if (value.toBool())
		{
			setMoveMode(true);
			_p1->show();
			_p2->show();
		}
	}
	return QGraphicsItem::itemChange(change, value);
}

void BezierEdgeImage::updateContextMenu(QList<QAction*> const & actionList)
{
	for (QAction* action : actionList)
	{
		if (action->text().compare("Linia krzywa") == 0)
		{
			action->setChecked(true);
		}
	}
}
