#include "EdgeImage.h"

EdgeImage::EdgeImage(VerticeImage * const verticeFrom, VerticeImage * const verticeTo)
: _verticeFrom(verticeFrom), _verticeTo(verticeTo)
{
	QPointF start = verticeFrom->pos();
	QPointF end = verticeTo->pos();
	VerticeContext context = verticeFrom->Context();
	int size = context.Size();
	if (std::abs(start.x() - end.x()) <= size &&
		std::abs(start.y() - end.y()) <= size)
	{
		_points.push_back(start);
		_points.push_back(QPointF(start.x() + 4 * size, start.y() - 6 * size));
		_points.push_back(QPointF(start.x() - 4 * size, start.y() - 6 * size));
		_points.push_back(end);
		_path.moveTo(_points[0]);
		_path.cubicTo(_points[1], _points[2], _points[3]);
	}
	else
	{
		_points.push_back(verticeFrom->pos());
		_points.push_back(verticeTo->pos());
	}
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
	//_line.setPoints(_verticeFrom->pos(), _verticeTo->pos());
	//painter->drawLine(_line);
	painter->drawPath(_path);
}


QRectF EdgeImage::boundingRect() const
{
	return QRectF(_verticeFrom->pos(), _verticeTo->pos()).normalized();
	return QRectF()
}
