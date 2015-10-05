#include "StraightEdgeImage.h"
#include "VertexImage.h"
#include "ArrowHeadImage.h"
#include "Config.h"

StraightEdgeImage::StraightEdgeImage(Edge * edge, VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context)
: EdgeImage(edge, vertexFrom, vertexTo, context)
{
	_line = QLineF(_vertexFrom->PointAt(_edge->Id()), _vertexTo->PointAt(_edge->Id()));
	calculateTextItemPos();
}


StraightEdgeImage::~StraightEdgeImage()
{
}

void StraightEdgeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	calculateNewLine();
	bool isArrow = Application::Config::Instance().IsGraphDirected();
	QPointF arrowCenter;
	if (isArrow && _arrow != nullptr)
	{
		_arrow->setRotation(-_line.angle() - 90);
		_arrow->setPos(_vertexTo->PointAt(_edge->Id()));
		_arrow->updateCenterPoint();
		arrowCenter = _arrow->Center();
	}
	calculateTextItemPos();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(_context->Color(), _context->Size(), Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	_line.setPoints(_vertexFrom->PointAt(_edge->Id()), !isArrow ? _vertexTo->PointAt(_edge->Id()) : arrowCenter);
	painter->drawLine(_line);
}

QRectF StraightEdgeImage::boundingRect() const
{
	return QRectF(_vertexFrom->PointAt(_edge->Id()), _vertexTo->PointAt(_edge->Id())).normalized();
}

void StraightEdgeImage::calculateTextItemPos()
{
	QPointF p1 = _line.p1();
	QPointF p2 = _line.p2();
	QPointF center = QPointF((p2.x() + p1.x()) / 2.0f, (p2.y() + p1.y()) / 2.0f);
	if (_text != nullptr)
		_text->setPos(center);
}
