#include "VertexImage.h"
#include "EdgeImage.h"
#include "Edge.h"

VertexImage::VertexImage(VertexContext const & context)
: _context(&context)
{
	setToolTip(QString("Notatka"));
	//	.arg(color.red()).arg(color.green()).arg(color.blue())
	//	.arg(static_cast<int>(scenePos().x())).arg(scenePos().y())
	//	.arg("Click and drag this color onto the robot!"));
	setCursor(Qt::OpenHandCursor);
	setAcceptedMouseButtons(Qt::LeftButton);
	setFlag(QGraphicsItem::ItemIsSelectable);
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
	vertex.addEllipse(QPointF(0.0f, 0.0f), _context->Size(), _context->Size());
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
		for (QMap<int, QPointF>::iterator it = _pointList.begin(); it != _pointList.end(); ++it)
		{
			(*it) += offset;
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
			_context = &Application::Config::Instance().SelectedVertexContext();
		}
		else
		{
			_context = &Application::Config::Instance().DefaultVertexContext();
		}
	}
	return QGraphicsItem::itemChange(change, value);
}

void VertexImage::addEdgePoint(EdgeImage * edge, VertexImage * vertex, bool first)
{
	float angle = edge->Angle();
	if (!first)
	{
		if (pos().y() < vertex->pos().y())
		{
			angle += 180;
		}
		else
		{
			angle -= 180;
		}
	}
	QPointF point = findPointOnCircle(angle);
	_pointList[edge->getEdge()->Id()] = point;
}

void VertexImage::setPointForEdge(int edgeId, float angle)
{
	_pointList[edgeId] = findPointOnCircle(angle);
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

