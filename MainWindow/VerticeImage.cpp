#include "VerticeImage.h"

VerticeImage::VerticeImage(VerticeContext const & context) : _context(context)
{
	setToolTip(QString("Cycusie"));
	//	.arg(color.red()).arg(color.green()).arg(color.blue())
	//	.arg(static_cast<int>(scenePos().x())).arg(scenePos().y())
	//	.arg("Click and drag this color onto the robot!"));
	setCursor(Qt::OpenHandCursor);
	setAcceptedMouseButtons(Qt::LeftButton);
	setFlag(QGraphicsItem::ItemIsSelectable);

	//_text = new TextItem(std::to_string(_vertice->Id()));
	//_text->setPos(startPoint.x(), startPoint.y());
	//_text->setBoundingRect(0, 0, context.Size() * 2, context.Size() * 2);
	//_text->replaceFont(context.Font());
}

QRectF VerticeImage::boundingRect() const
{
	int size = _context.Size() * 2;
	return QRectF(-_context.Size(), -_context.Size(), size, size);
}

void VerticeImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setRenderHint(QPainter::RenderHint::HighQualityAntialiasing);

	QPainterPath vertice;
	vertice.addEllipse(QPointF(0.0f, 0.0f), _context.Size(), _context.Size());

	QPointF point = pos();

	painter->setPen(QPen(_context.StrokeColor(), _context.StrokeSize(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter->setBrush(_context.Color());
	painter->drawPath(vertice);
	painter->setFont(_context.Font());
	painter->setPen(QColor(0, 0, 0));
	painter->drawText(QRectF(-_context.Size(), -_context.Size(), _context.Size() * 2, _context.Size() * 2), QString::number(_vertice->Id()), QTextOption(Qt::AlignCenter));
}

void VerticeImage::select(bool selected)
{
	setSelected(selected);
	if (isSelected())
		_context = Application::Config::Instance().SelectedVerticeContext();
	else
		_context = Application::Config::Instance().DefaultVerticeContext();
}

void VerticeImage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if ((flags() | ItemIsMovable) == flags())
	{
		setCursor(Qt::ClosedHandCursor);
		offset = event->scenePos();
	}
}

void VerticeImage::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if ((flags() | ItemIsMovable) == flags())
	{
		setPos(scenePos() + event->scenePos() - offset);
		offset = event->scenePos();
	}
}

void VerticeImage::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if ((flags() | ItemIsMovable) == flags())
	{
		setCursor(Qt::OpenHandCursor);
	}
}
