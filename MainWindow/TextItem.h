#pragma once
#include <QGraphicsTextItem>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QPainter>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <regex>

class TextItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	TextItem(QPointF const & point, QGraphicsItem* parent = 0);
	TextItem(const QString& text, QGraphicsItem* parent = 0);
	TextItem(TextItem const & other);

	QRectF boundingRect() const  Q_DECL_OVERRIDE;
	void setBoundingRect(qreal x, qreal y, qreal w, qreal h);
	void setBoundingRect(QRect const & rect);
	void setAlignment(Qt::AlignmentFlag flag);
	void updateBoundingRect();
	void setRegex(std::string const & pattern);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
	QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	void init();
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	virtual void turnOffEditorMode();
	void turnOnEditorMode(bool selectAll);

public:
	void replaceFont(QFont const & font);
	void setText(QString const & text);
	void setTextInteraction(bool on, bool selectAll = false);

	inline QString getText() const { return _text; }
	
protected:
	QRectF _rect;
	QTextOption _option;
	QString _text;
	QFont _font;
	QColor _textColor;
	QGraphicsTextItem * _textEdit;
	std::regex _regex;
	QString _oldStr;

signals:
	void valueChanged(QString const &);
};
