#include "LoopEdgeImage.h"

LoopEdgeImage::LoopEdgeImage(VertexImage * const vertexFrom, VertexImage * const vertexTo, EdgeContext const & context)
: EdgeImage(vertexFrom, vertexTo, context)
{
	QPointF start = vertexFrom->pos();
	QPointF end = vertexTo->pos();
	int size = vertexFrom->Context().Size();
	_points.push_back(start);
	_points.push_back(QPointF(start.x() + 4 * size, start.y() - 6 * size));
	_points.push_back(QPointF(start.x() - 4 * size, start.y() - 6 * size));
	_points.push_back(end);
}


LoopEdgeImage::~LoopEdgeImage()
{
}

void LoopEdgeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(QPen(QColor(0, 0, 0), 10, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

	if (_points[0] != _vertexFrom->pos())
	{
		QPointF dp = _vertexFrom->pos() - _points[0];
		for (std::vector<QPointF>::iterator it = _points.begin(); it != _points.end(); ++it)
		{
			*it += dp;
		}
	}
	_path = QPainterPath();
	_path.moveTo(_points[0]);
	_path.cubicTo(_points[1], _points[2], _points[3]);
	painter->drawPath(_path);
}

QRectF LoopEdgeImage::boundingRect() const
{
	return _path.boundingRect();
}
