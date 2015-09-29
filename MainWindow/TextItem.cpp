#include "TextItem.h"

TextItem::TextItem(QGraphicsItem* parent) : QGraphicsTextItem(parent)
{
	init();
}

TextItem::TextItem(const QString& text, QGraphicsItem* parent) : QGraphicsTextItem(text, parent)
{
	init();
	_text = text;
}

void TextItem::init()
{
	_option.setAlignment(Qt::AlignLeft);
	_option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
}

void TextItem::replaceFont(QFont const & font)
{
	_font = font;
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
	painter->setFont(_font);
	painter->drawText(boundingRect(), _text, _option);
}
