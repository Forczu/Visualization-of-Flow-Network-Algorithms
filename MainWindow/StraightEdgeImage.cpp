#include "StraightEdgeImage.h"


StraightEdgeImage::StraightEdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context)
: EdgeImage(vertexFrom, vertexTo, context)
{
	_line = QLineF(vertexFrom->pos(), vertexTo->pos());
}


StraightEdgeImage::~StraightEdgeImage()
{
}

void StraightEdgeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(_context.Color(), _context.Size(), Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	_line.setPoints(_vertexFrom->pos(), _vertexTo->pos());
	painter->drawLine(_line);
}

QRectF StraightEdgeImage::boundingRect() const
{
	return QRectF(_vertexFrom->pos(), _vertexTo->pos()).normalized();
}
