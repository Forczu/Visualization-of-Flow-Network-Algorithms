#pragma once
#include <QTabWidget>
#include "GraphView.h"
#include "Orders.h"
#include "Weights.h"

class GraphTabWidget : public QTabWidget
{
	Q_OBJECT

	std::map<int, QString> _tabNameMap;

public:
	GraphTabWidget(QWidget * parent);
	~GraphTabWidget();

	void addTab(QString const & name, Order order, Weight weighted);
	void addTab(QString const & name, GraphImage * graph);
	void createTab(GraphView * widget, QString const & name);

	inline GraphView * currentGraphView() const
	{
		GraphView * view = dynamic_cast<GraphView*>(currentWidget());
		return view != NULL ? view : nullptr;
	}

private:
	QString getTabName(int index, QString const & scaleStr);
	QString getPercentScale(float scaleFactor);

private slots:
	void closeTab(int index);
	void viewChanged();
	void changeScale(float scaleFactor);

signals:
	void graphChanged();
};

