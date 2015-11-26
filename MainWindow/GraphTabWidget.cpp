#include "GraphTabWidget.h"
#include <QLayout>
#include "GraphScene.h"
#include "GraphImage.h"

GraphTabWidget::GraphTabWidget(QWidget * parent) : QTabWidget(parent)
{
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	setTabsClosable(true);
	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

GraphTabWidget::~GraphTabWidget()
{
	int tabNumber = count();
	for (int i = tabNumber; i; --i)
	{
		closeTab(i);
	}
}

void GraphTabWidget::addTab(GraphImage * graph)
{
	GraphView * view = createView(graph);
	createTab(view, graph->getName());
	view->centerOn(graph);
}

void GraphTabWidget::addTab(GraphImage * graph, QString const & name)
{
	GraphView * view = createView(graph);
	createTab(view, name);
	graph->setName(name);
}

GraphView * GraphTabWidget::createView(GraphImage * graph)
{
	GraphView * view = new GraphView(graph);
	view->addScene(GraphScene::getInstance());
	return view;
}

void GraphTabWidget::createTab(GraphView * widget, QString const & name)
{
	QString scaleStr = getPercentScale(widget->getScale());
	int id = QTabWidget::addTab(widget, name + " @ " + scaleStr);
	_tabNameMap[id] = name;
	setCurrentWidget(widget);
	connect(widget, SIGNAL(graphChanged()), this, SLOT(viewChanged()));
	connect(widget, SIGNAL(scaleChanged(float)), this, SLOT(changeScale(float)));
}

void GraphTabWidget::closeTab(int index)
{
	auto widget = QTabWidget::widget(index);
	if (widget) delete widget;
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
