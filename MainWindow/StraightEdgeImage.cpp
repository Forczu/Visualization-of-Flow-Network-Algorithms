#include "StraightEdgeImage.h"
#include "VertexImage.h"
#include "ArrowHeadImage.h"
#include "Config.h"
#include "TextItem.h"
#include "Edge.h"

StraightEdgeImage::StraightEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context)
: EdgeImage(edge, vertexFrom, vertexTo, context)
{
	_line = QLineF(_vertexFrom->PointAt(_edge->Id()), _vertexTo->PointAt(_edge->Id()));
}


StraightEdgeImage::~StraightEdgeImage()
{
}

void StraightEdgeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	calculateNewLine();
	QPointF oldCenter = _center;
	updateCenterPoint();
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
		_arrow->setRotation(-_line.angle() - 90);
		_arrow->setPos(_vertexTo->PointAt(_edge->Id()));
		_arrow->updateCenterPoint();
		arrowCenter = _arrow->Center();
	}
	_line.setPoints(_vertexFrom->PointAt(_edge->Id()), !isArrow ? _vertexTo->PointAt(_edge->Id()) : arrowCenter);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(_context->Color(), _context->Size(), Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	painter->drawLine(_line);
}

QRectF StraightEdgeImage::boundingRect() const
{
	return QRectF(_vertexFrom->PointAt(_edge->Id()), _vertexTo->PointAt(_edge->Id())).normalized();
}

void StraightEdgeImage::updateCenterPoint()
{
	QPointF p1 = _line.p1();
	QPointF p2 = _line.p2();
	_center = QPointF((p2.x() + p1.x()) / 2.0f, (p2.y() + p1.y()) / 2.0f);
}
