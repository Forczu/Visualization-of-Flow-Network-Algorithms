#include "GraphScrollBar.h"

GraphScrollBar::GraphScrollBar(QWidget *parent) : QScrollBar(parent)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateMask()));
}

void GraphScrollBar::updateMask()
{
	QStyleOptionSlider opt;
	initStyleOption(&opt);

	QRegion r(style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSlider, this));
	r += style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarAddLine, this);
	r += style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSubLine, this);
	setMask(r);
}

void GraphScrollBar::showEvent(QShowEvent * event)
{
	QScrollBar::showEvent(event);
	updateMask();
}