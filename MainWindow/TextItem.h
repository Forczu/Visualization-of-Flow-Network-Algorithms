#pragma once

#include <QGraphicsTextItem>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QPainter>
#include <QFont>

class TextItem : public QGraphicsTextItem
{
public:
	TextItem(QGraphicsItem* parent = 0);
	TextItem(const QString& text, QGraphicsItem* parent = 0);

	QRectF boundingRect() const  Q_DECL_OVERRIDE
	{
		return _rect;
	}

	void setBoundingRect(qreal x, qreal y, qreal w, qreal h);
	void setBoundingRect(QRect const & rect);
	void setAlignment(Qt::AlignmentFlag flag);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
	void init();
public:
	void replaceFont(QFont const & font);
	void setText(QString const & text);

private:
	QRectF _rect;
	QTextOption _option;
	QString _text;
	QFont _font;
};

