#include "GraphTabWidget.h"
#include <QLayout>

GraphTabWidget::GraphTabWidget(QWidget * parent) : QTabWidget(parent)
{
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	setTabsClosable(true);
	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

GraphTabWidget::~GraphTabWidget()
{
}

void GraphTabWidget::addTab(QString const & name, Order order, Weight weighted)
{
	GraphView * widget = new GraphView(order, weighted);
	QString scaleStr = getPercentScale(widget->getScale());
	int id = QTabWidget::addTab(widget, name + " @ " + scaleStr);
	_tabNameMap[id] = name;
	setCurrentWidget(widget);
	connect(widget, SIGNAL(graphChanged()), this, SLOT(viewChanged()));
	connect(widget, SIGNAL(scaleChanged(float)), this, SLOT(changeScale(float)));
}

void GraphTabWidget::closeTab(int index)
{
	removeTab(index);
	if (count() == 0)
		hide();
}

void GraphTabWidget::viewChanged()
{
	emit graphChanged();
}

void GraphTabWidget::changeScale(float scaleFactor)
{
	QString scaleStr = getPercentScale(scaleFactor);
	int index = currentIndex();
	setTabText(index, getTabName(index, scaleStr));
}

QString GraphTabWidget::getTabName(int index, QString const & scaleStr)
{
	return QString(_tabNameMap[index] + " @ " + scaleStr);
}

QString GraphTabWidget::getPercentScale(float scaleFactor)
{
	return QString("%1%").arg(static_cast<int>(scaleFactor * 100));
}
