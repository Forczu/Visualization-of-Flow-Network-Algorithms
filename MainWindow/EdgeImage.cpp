#include "EdgeImage.h"

EdgeImage::EdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo)
: _vertexFrom(vertexFrom), _vertexTo(vertexTo)
{
	//QPointF start = vertexFrom->pos();
	//QPointF end = vertexTo->pos();
	//VertexContext context = vertexFrom->Context();
	//int size = context.Size();
	//if (std::abs(start.x() - end.x()) <= size &&
	//	std::abs(start.y() - end.y()) <= size)
	//{
	//	_points.push_back(start);
	//	_points.push_back(QPointF(start.x() + 4 * size, start.y() - 6 * size));
	//	_points.push_back(QPointF(start.x() - 4 * size, start.y() - 6 * size));
	//	_points.push_back(end);
	//	//_path.moveTo(_points[0]);
	//	//_path.cubicTo(_points[1], _points[2], _points[3]);
	//}
	//else
	//{
	//	_points.push_back(vertexFrom->pos());
	//	_points.push_back(vertexTo->pos());
	//}
}

EdgeImage::~EdgeImage()
{
}

void EdgeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(QColor(0, 0, 0), 10, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	//for (std::vector<QPointF>::const_iterator it = _points.begin(); it != _points.end(); ++it)
	//{
	//}
	//_line.setPoints(_vertexFrom->pos(), _vertexTo->pos());
	//painter->drawLine(_line);
	//painter->drawPath(_path);
}


QRectF EdgeImage::boundingRect() const
{
	return QRectF(_vertexFrom->pos(), _vertexTo->pos()).normalized();
	//return QRectF()
}
