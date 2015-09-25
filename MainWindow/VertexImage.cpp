#include "VertexImage.h"

VertexImage::VertexImage(VertexContext const & context) : _context(context)
{
	setToolTip(QString("Cycusie"));
	//	.arg(color.red()).arg(color.green()).arg(color.blue())
	//	.arg(static_cast<int>(scenePos().x())).arg(scenePos().y())
	//	.arg("Click and drag this color onto the robot!"));
	setCursor(Qt::OpenHandCursor);
	setAcceptedMouseButtons(Qt::LeftButton);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setSelected(false);

	//_text = new TextItem(std::to_string(_vertice->Id()));
	//_text->setPos(startPoint.x(), startPoint.y());
	//_text->setBoundingRect(0, 0, context.Size() * 2, context.Size() * 2);
	//_text->replaceFont(context.Font());
}

QRectF VertexImage::boundingRect() const
{
	int size = _context.Size() * 2;
	return QRectF(-_context.Size(), -_context.Size(), size, size);
}

void VertexImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setRenderHint(QPainter::RenderHint::HighQualityAntialiasing);
	QPainterPath vertex;
	vertex.addEllipse(QPointF(0.0f, 0.0f), _context.Size(), _context.Size());
	painter->setPen(QPen(_context.StrokeColor(), _context.StrokeSize(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter->setBrush(_context.Color());
	painter->drawPath(vertex);
	painter->setFont(_context.Font());
	painter->setPen(QColor(0, 0, 0));
	painter->drawText(QRectF(-_context.Size(), -_context.Size(), _context.Size() * 2, _context.Size() * 2), QString::number(_vertex->Id()), QTextOption(Qt::AlignCenter));
}

void VertexImage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if ((flags() | ItemIsMovable) == flags())
	{
		setCursor(Qt::ClosedHandCursor);
		offset = event->scenePos();
	}
}

void VertexImage::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if ((flags() | ItemIsMovable) == flags() && isSelected())
	{
		setPos(scenePos() + event->scenePos() - offset);
		offset = event->scenePos();
	}
}

void VertexImage::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if ((flags() | ItemIsMovable) == flags())
	{
		setCursor(Qt::OpenHandCursor);
	}
}

QVariant VertexImage::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange && scene()) {
		// value is the new position.
		QPointF newPos = value.toPointF();
		QRectF rect = scene()->sceneRect();
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
			_context = Application::Config::Instance().SelectedVertexContext();
		}
		else
		{
			_context = Application::Config::Instance().DefaultVertexContext();
		}
	}
	return QGraphicsItem::itemChange(change, value);
}
