#include "VertexImage.h"
#include "EdgeImage.h"
#include "Edge.h"

VertexImage::VertexImage(VertexContext * context) : _context(context)
{
	setCursor(Qt::OpenHandCursor);
	setAcceptedMouseButtons(Qt::LeftButton);
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
	setSelected(false);
}

VertexImage::~VertexImage()
{
}

QRectF VertexImage::boundingRect() const
{
	int size = _context->Size() * 2;
	return QRectF(-_context->Size(), -_context->Size(), size, size);
}

void VertexImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setRenderHint(QPainter::RenderHint::HighQualityAntialiasing);
	QBrush brush;
	brush.setColor(_context->Color());
	brush.setStyle(Qt::Dense2Pattern);
	painter->setBrush(brush);
	QPainterPath vertex;
	vertex.addEllipse(QPointF(), _context->Size(), _context->Size());
	painter->setPen(QPen(_context->StrokeColor(), _context->StrokeSize(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter->drawPath(vertex);
	painter->setFont(_context->Font());
	painter->setPen(QColor(0, 0, 0));
	painter->drawText(QRectF(-_context->Size(), -_context->Size(), _context->Size() * 2, _context->Size() * 2), QString::number(_vertex->Id()), QTextOption(Qt::AlignCenter));
}

QVariant VertexImage::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange && scene()) {
		// value is the new position.
		QPointF newPos = value.toPointF();
		QRectF rect = scene()->sceneRect();
		QPointF offset = newPos - pos();
		for (PointMap::iterator it = _pointList.begin(); it != _pointList.end(); ++it)
		{
			(*it).second += offset;
		}
		if (!rect.contains(newPos)) {
			// Keep the item inside the scene rect.
			newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
			newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
			return newPos;
		}
	}
	else if (change == ItemSelectedChange)
	{
		if (value.toBool() == true)
		{
			_context = getParent()->getConfig()->SelectedVertexContext();
		}
		else
		{
			_context = getParent()->getConfig()->NormalVertexContext();
		}
	}
	return QGraphicsItem::itemChange(change, value);
}

void VertexImage::addEdgePoint(EdgeImage * edge, VertexImage * vertex, bool first)
{
	float angle = edge->getAngle();
	if (!first)
	{
		if (pos().y() < vertex->pos().y())
			angle += 180;
		else
			angle -= 180;
	}
	QPointF point = findPointOnCircle(angle);
	_pointList[edge->getEdge()->Id()] = point;
}

void VertexImage::setPointForEdge(int edgeId, float angle)
{
	_pointList[edgeId] = findPointOnCircle(angle);
}

void VertexImage::setToolTip(int degree)
{
	QGraphicsItem::setToolTip(QString("Stopieñ: %1").arg(degree));
}

void VertexImage::setToolTip(int indegree, int outdegree)
{
	QGraphicsItem::setToolTip(QString::fromUtf8(
		"Stopieñ wejœciowy: %1\nStopieñ wyjœciowy: %2\nSuma stopni: %3")
		.arg(indegree).arg(outdegree).arg(indegree + outdegree));
}

QPointF VertexImage::findPointOnCircle(float angle)
{
	const float ANGLE_INTERVAL = 10.0f;
	float foundAngle = 0.0f;
	for (int i = 0; i < 360; i += ANGLE_INTERVAL)
	{
		if (i < angle - 2.0f)
			continue;
		foundAngle = i;
		break;
	}
	return QPointF(
		pos().x() + Context()->Size() * std::cos(-foundAngle * M_PI / 180.0f),
		pos().y() + Context()->Size() * std::sin(-foundAngle * M_PI / 180.0f));
}

