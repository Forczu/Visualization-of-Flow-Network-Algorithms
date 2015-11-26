#include "StraightEdgeImage.h"
#include "VertexImage.h"
#include "ArrowHeadImage.h"
#include "Config.h"
#include "TextItem.h"
#include "Edge.h"

StraightEdgeImage::StraightEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext * context)
: EdgeImage(edge, vertexFrom, vertexTo, context)
{
	QPointF first = _vertexFrom->PointAt(_edge->Id());
	setPos(first);
	QPointF second = _vertexTo->PointAt(getEdge()->Id()) - pos();
	_line = QLineF(QPointF(), second);
	checkNewLine();
	setCenterPoint();
}


StraightEdgeImage::~StraightEdgeImage()
{
}

void StraightEdgeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	static qreal oldAngle = _line.angle();
	checkNewLine();
	QPointF oldCenter = _center;
	setCenterPoint();
	qreal currAngle = _line.angle();
	if (_text != nullptr)
	{
		if (currAngle <= oldAngle - 0.01 || currAngle >= oldAngle + 0.01)
		{
			int dx = _center.x() - oldCenter.x();
			int dy = _center.y() - oldCenter.y();
			_text->moveBy(dx, dy);
		}
		if (_text->isSelected())
		{
			QLineF connection = QLineF(_center, _text->pos());
			painter->setPen(QPen(Qt::black, 2, Qt::DotLine));
			painter->drawLine(connection);
		}
	}
	bool isArrow = _arrow != nullptr;
	QPointF arrowCenter;
	if (isArrow)
	{
		_arrow->setRotation(-currAngle - 90);
		_arrow->setPos(VertexTo()->PointAt(getEdge()->Id()) - pos());
		_arrow->updateCenterPoint();
		arrowCenter = _arrow->Center();
	}
	oldAngle = currAngle;
	_line.setPoints(QPointF(), !isArrow ? _vertexTo->PointAt(_edge->Id()) : arrowCenter);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(_context->Color(), _context->Size(), Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	painter->drawLine(_line);
#if DEBUG
	painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	painter->drawPath(shape());
#endif
}

QRectF StraightEdgeImage::boundingRect() const
{
	return QRectF(QPointF(), _arrow == nullptr ? _vertexTo->PointAt(_edge->Id()) : _arrow->Center()).normalized();
}

QPainterPath StraightEdgeImage::shape() const
{
	QPainterPath path;
	QPolygon polygon;
	QPoint firstPoint = _line.p1().toPoint();
	QPoint secondPoint = _line.p2().toPoint();
	float angle = _line.angle();
	int size = _context->Size() / 2;
	float angle1 = MyMath::toRadian(angle);
	float angle2 = MyMath::toRadian(angle + 180.0f);
	QPoint p1 = QPoint(firstPoint.x() + std::sin(angle1) * size, firstPoint.y() + std::cos(angle1) * size);
	QPoint p2 = QPoint(firstPoint.x() + std::sin(angle2) * size, firstPoint.y() + std::cos(angle2) * size);
	QPoint p3 = QPoint(secondPoint.x() + std::sin(angle1) * size, secondPoint.y() + std::cos(angle1) * size);
	QPoint p4 = QPoint(secondPoint.x() + std::sin(angle2) * size, secondPoint.y() + std::cos(angle2) * size);
	polygon << p1 << p2 << p4 << p3 << p1;
	path.addPolygon(polygon);
	return path;
}

void StraightEdgeImage::setCenterPoint()
{
	QPointF p1 = _line.p1();
	QPointF p2 = _line.p2();
	_center = QPointF((p2.x() + p1.x()) / 2.0f, (p2.y() + p1.y()) / 2.0f);
}


void StraightEdgeImage::updateContextMenu(QList<QAction*> const & actionList)
{
	for (QAction* action : actionList)
	{
		if (action->text().compare("Linia prosta") == 0)
		{
			action->setChecked(true);
			break;
		}
	}
}
