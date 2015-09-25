#include "StraightEdgeImage.h"


StraightEdgeImage::StraightEdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo)
: EdgeImage(vertexFrom, vertexTo)
{
	_line = QLineF(vertexFrom->pos(), vertexTo->pos());
}


StraightEdgeImage::~StraightEdgeImage()
{
}

void StraightEdgeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(QColor(0, 0, 0), 10, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	_line.setPoints(_vertexFrom->pos(), _vertexTo->pos());
	painter->drawLine(_line);
}

QRectF StraightEdgeImage::boundingRect() const
{
	return QRectF(_vertexFrom->pos(), _vertexTo->pos()).normalized();
}
