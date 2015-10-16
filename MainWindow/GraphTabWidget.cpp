#include "GraphTabWidget.h"

GraphTabWidget::GraphTabWidget(QWidget * parent /*= 0*/) : QTabWidget(parent)
{
	setGeometry(230, 120, 771, 581);
	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(changeTab(int)));
}

GraphTabWidget::~GraphTabWidget()
{
}

void GraphTabWidget::addTab(QString const & name, Order order, Weight weighted)
{
	GraphView * widget = new GraphView(order, weighted);
	QTabWidget::addTab(widget, name);
	setCurrentWidget(widget);
	connect(widget, SIGNAL(graphChanged()), this, SLOT(viewChanged()));
}

void GraphTabWidget::closeTab(int index)
{
	removeTab(index);
}

void GraphTabWidget::viewChanged()
{
	emit graphChanged();
}
