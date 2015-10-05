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
	auto items = childItems();
	QPointF arrowCenter;
	if (items.size() != 0)
	{
		std::for_each(items.begin(), items.end(), [&](QGraphicsItem * item)
		{
			ArrowHeadImage * arrow = dynamic_cast<ArrowHeadImage *>(item);
			if (NULL != arrow)
			{
				arrow->setRotation(-_line.angle() - 90);
				arrow->setPos(_vertexTo->PointAt(_edge->Id()));
				arrow->updateCenterPoint();
				arrowCenter = arrow->Center();
			}
		});
	}
	calculateTextItemPos();

	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(_context->Color(), _context->Size(), Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	_line.setPoints(_vertexFrom->PointAt(_edge->Id()), arrowCenter);
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
