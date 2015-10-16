#pragma once
#include <QTabWidget>
#include "GraphView.h"
#include "Orders.h"
#include "Weights.h"

class GraphTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	GraphTabWidget(QWidget * parent = 0);
	~GraphTabWidget();

	void addTab(QString const & name, Order order, Weight weighted);

	inline GraphView * currentGraphView() const
	{
		GraphView * view = dynamic_cast<GraphView*>(currentWidget());
		return view != NULL ? view : nullptr;
	}

private slots:
	void closeTab(int index);
	void viewChanged();

signals:
	void graphChanged();
};

