#include "VertexImage.h"

VertexImage::VertexImage(VertexContext const & context)
: _context(context), _vertex(nullptr), _edgeLabel(EdgeLabel::None)
{
	setToolTip(QString("Cycusie"));
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
	int size = _context.Size() * 2;
	return QRectF(-_context.Size(), -_context.Size(), size, size);
}

void VertexImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setRenderHint(QPainter::RenderHint::HighQualityAntialiasing);
	QBrush brush;
	brush.setColor(_context.Color());
	brush.setStyle(Qt::DiagCrossPattern);
	painter->setBrush(brush);
	QPainterPath vertex;
	vertex.addEllipse(QPointF(0.0f, 0.0f), _context.Size(), _context.Size());
	painter->setPen(QPen(_context.StrokeColor(), _context.StrokeSize(), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter->drawPath(vertex);
	painter->setFont(_context.Font());
	painter->setPen(QColor(0, 0, 0));
	painter->drawText(QRectF(-_context.Size(), -_context.Size(), _context.Size() * 2, _context.Size() * 2), QString::number(_vertex->Id()), QTextOption(Qt::AlignCenter));

	QFont font;
	font.setBold(true);
	font.setItalic(true);
	font.setPointSize(16);
	font.setFamily(QString("Calibri"));
	painter->setFont(font);
	painter->setPen(QColor(0, 0, 0));
	painter->drawText(QRectF(-_context.Size(), -(_context.Size() + 40), _context.Size() * 2, 40), _label, QTextOption(Qt::AlignLeft));
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

void VertexImage::setEdgeLabel(EdgeLabel val)
{
	_edgeLabel = val;
	switch (_edgeLabel)
	{
	case EdgeLabel::None:
		_label = QString();
		break;
	case EdgeLabel::Source:
		_label = QString("Source");
		break;
	case EdgeLabel::Target:
		_label = QString("Target");
		break;
	default:
		break;
	}
}
