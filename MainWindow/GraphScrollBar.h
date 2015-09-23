#pragma once

#include <QScrollBar>
#include <QStyle>
#include <QStyleOptionSlider>

class GraphScrollBar : public QScrollBar
{
public:
	explicit GraphScrollBar(QWidget *parent = 0);

protected:
	void showEvent(QShowEvent * event);

signals:
	public slots :
		void updateMask();
};

