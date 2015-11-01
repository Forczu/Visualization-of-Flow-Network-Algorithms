#include "TextItem.h"
#include <QPalette>
#include <QKeyEvent>

TextItem::TextItem(QPointF const & point, QGraphicsItem* parent) : QGraphicsItem(parent)
{
	setPos(point);
	init();
}

TextItem::TextItem(const QString& text, QGraphicsItem* parent) : QGraphicsItem(parent)
{
	_text = text;
	init();
}

QRectF TextItem::boundingRect() const
{
	return _rect;
}

void TextItem::init()
{
	_option.setAlignment(Qt::AlignLeft);
	_option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
	setAcceptedMouseButtons(Qt::LeftButton);
	setSelected(false);

	_textEdit = new QGraphicsTextItem(this);
	_textEdit->setTextInteractionFlags(Qt::NoTextInteraction);
	_textEdit->setPlainText(_text);
	updateBoundingRect();
}

void TextItem::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key::Key_Return &&
		_textEdit->textInteractionFlags() == Qt::TextEditorInteraction)
	{
		setTextInteraction(false);
	}
}

void TextItem::replaceFont(QFont const & font)
{
	_font = font;
	_textEdit->setFont(_font);
	updateBoundingRect();
}

void TextItem::setText(QString const & text)
{
	_text = text;
	_textEdit->setPlainText(_text);
	updateBoundingRect();
}

void TextItem::updateBoundingRect()
{
	setBoundingRect(_textEdit->boundingRect().toRect());
}

void TextItem::setRegex(std::string const & pattern)
{
	_regex.assign(pattern);
}

void TextItem::setBoundingRect(qreal x, qreal y, qreal w, qreal h)
{
	_rect.setRect(x, y, w, h);
}

void TextItem::setBoundingRect(QRect const & rect)
{
	_rect = rect;
}

void TextItem::setAlignment(Qt::AlignmentFlag flag)
{
	_option.setAlignment(flag);
}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	painter->setRenderHint(QPainter::Antialiasing);
	if (isSelected() && _textEdit->textInteractionFlags() != Qt::TextEditorInteraction)
		painter->setPen(Qt::DotLine);
	else
		painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(Qt::white));
	painter->drawRoundedRect(_rect, 10, 10);
}

void TextItem::setTextInteraction(bool on, bool selectAll)
{
	if (on && _textEdit->textInteractionFlags() == Qt::NoTextInteraction)
	{
		turnOnEditorMode(selectAll);
	}
	else if (!on && _textEdit->textInteractionFlags() == Qt::TextEditorInteraction)
	{
		turnOffEditorMode();
	}
}

void TextItem::turnOffEditorMode()
{
	// turn off editor mode:
	_textEdit->setTextInteractionFlags(Qt::NoTextInteraction);
	// deselect text (else it keeps gray shade):
	QTextCursor c = _textEdit->textCursor();
	c.clearSelection();
	_textEdit->setTextCursor(c);
	clearFocus();
	updateBoundingRect();
	_text = _textEdit->toPlainText();
}

void TextItem::turnOnEditorMode(bool selectAll)
{
	// switch on editor mode:
	_textEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
	// manually do what a mouse click would do else:
	_textEdit->setFocus(Qt::MouseFocusReason); // this gives the item keyboard focus
	_textEdit->setSelected(true); // this ensures that itemChange() gets called when we click out of the item
	if (selectAll) // option to select the whole text (e.g. after creation of the TextItem)
	{
		QTextCursor c = _textEdit->textCursor();
		c.select(QTextCursor::Document);
		_textEdit->setTextCursor(c);
	}
	_oldStr = _text;
}

void TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	if (_textEdit->textInteractionFlags() == Qt::TextEditorInteraction)
	{
		// if editor mode is already on: pass double click events on to the editor:
		QGraphicsItem::mouseDoubleClickEvent(event);
		return;
	}

	// if editor mode is off:
	// 1. turn editor mode on and set selected and focused:
	setTextInteraction(true);

	// 2. send a single click to this QGraphicsTextItem (this will set the cursor to the mouse position):
	// create a new mouse event with the same parameters as event
	QGraphicsSceneMouseEvent * click = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
	click->setButton(event->button());
	click->setPos(event->pos());
	QGraphicsItem::mousePressEvent(click);
	delete click; // don't forget to delete the event
}

QVariant TextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedChange
		&& _textEdit->textInteractionFlags() != Qt::NoTextInteraction
		&& !value.toBool())
	{
		setTextInteraction(false); // leave editor mode
		std::string currStr = _textEdit->toPlainText().toStdString();
		if (!std::regex_match(currStr, _regex))
		{
			_textEdit->setPlainText(_oldStr);
			setTextInteraction(true);
			setTextInteraction(false);
		}
	}
	return QGraphicsItem::itemChange(change, value);
}
