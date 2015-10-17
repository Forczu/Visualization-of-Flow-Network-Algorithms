#include "mainwindow.h"
#include "VertexImage.h"
#include "GraphShapeDialog.h"
#include "Config.h"
#include "Vertex.h"
#include "StraightEdgeImage.h"
#include "LoopEdgeImage.h"
#include "GraphView.h"
#include "GraphImage.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	ui.setupUi(this);

	createActions();

	_tools[Tool::Vertex]		= ui.actionAddVertex;
	_tools[Tool::Edge]			= ui.actionAddEdge;
	_tools[Tool::Grab]			= ui.actionGrab;
	_tools[Tool::RubberBand]	= ui.actionSelect;
	_tools[Tool::Pointer]		= ui.actionPointer;
	_tools[Tool::Remove]		= ui.actionRemove;

	ui.actionPointer->setChecked(true);
	_graphTabs.setParent(this);
	_graphTabs.show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
}

void MainWindow::newFile()
{
	int index = _graphTabs.count() + 1;
	CreateNewGraphDialog dialog(index);
	dialog.show();
	dialog.exec();
	if (!dialog.Confirmed())
		return;
	_graphTabs.addTab(dialog.getName(), dialog.getOrder(), dialog.getWeighted());
}

void MainWindow::close()
{
	QApplication::quit();
}

void MainWindow::checkAddVertexButton(bool b)
{
	checkButton(Tool::Vertex, b);
	setCursorForWidget(_graphTabs.currentWidget(), Qt::ArrowCursor);
}

void MainWindow::setCursorForWidget(QWidget * widget, Qt::CursorShape shape)
{
	if (!widget)
		return;
	widget->setCursor(shape);
}

void MainWindow::checkAddEdgeButton(bool b)
{
	checkButton(Tool::Edge, b);
	GraphView * currentView = _graphTabs.currentGraphView();
	if (currentView == nullptr)
		return;
	currentView->setCursor(Qt::ArrowCursor);
	if (b)
		currentView->setEdgeFlag(EdgeFlag::Source);
	else
		currentView->setEdgeFlag(EdgeFlag::None);
	currentView->AddEdgeFlag(b);
}

void MainWindow::checkGrabButton(bool b)
{
	checkButton(Tool::Grab, b);
	setCursorForWidget(_graphTabs.currentWidget(), Qt::OpenHandCursor);
}

void MainWindow::checkSelectionButton(bool b)
{
	checkButton(Tool::RubberBand, b);
}

void MainWindow::checkPointerButton(bool b)
{
	checkButton(Tool::Pointer, b);
	setCursorForWidget(_graphTabs.currentWidget(), Qt::ArrowCursor);
}

void MainWindow::checkRemoveButton(bool b)
{
	checkButton(Tool::Remove, b);
	setCursorForWidget(_graphTabs.currentWidget(), Qt::CrossCursor);
}

void MainWindow::openGraphShapeDialog()
{
	GraphShapeDialog graphShapeDialog = GraphShapeDialog(this);
	graphShapeDialog.setModal(false);
	graphShapeDialog.exec();
}

void MainWindow::createActions()
{
	ui.actionClose->setShortcuts(QKeySequence::Close);
	ui.actionClose->setStatusTip(tr("Zamyka program"));

	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionShape, SIGNAL(triggered()), this, SLOT(openGraphShapeDialog()));

	connect(&_graphTabs, SIGNAL(currentChanged(int)), this, SLOT(updateGraphStatus()));
	connect(&_graphTabs, SIGNAL(graphChanged()), this, SLOT(updateGraphStatus()));

	connect(ui.actionAddVertex, SIGNAL(triggered(bool)), this, SLOT(checkAddVertexButton(bool)));
	connect(ui.actionAddEdge, SIGNAL(triggered(bool)), this, SLOT(checkAddEdgeButton(bool)));
	connect(ui.actionGrab, SIGNAL(triggered(bool)), this, SLOT(checkGrabButton(bool)));
	connect(ui.actionSelect, SIGNAL(triggered(bool)), this, SLOT(checkSelectionButton(bool)));
	connect(ui.actionPointer, SIGNAL(triggered(bool)), this, SLOT(checkPointerButton(bool)));
	connect(ui.actionRemove, SIGNAL(triggered(bool)), this, SLOT(checkRemoveButton(bool)));
	connect(ui.actionDirectedGraph, SIGNAL(triggered(bool)), this, SLOT(clickOrderDirected(bool)));
	connect(ui.actionUndirectedGraph, SIGNAL(triggered(bool)), this, SLOT(clickOrderUndirected(bool)));
	connect(ui.actionWeightedGraph, SIGNAL(triggered(bool)), this, SLOT(clickWeighted(bool)));
	connect(ui.actionUnweightedGraph, SIGNAL(triggered(bool)), this, SLOT(clickUnweighted(bool)));
}

void MainWindow::checkButton(Tool tool, bool b)
{
	if (b)
	{
		Application::Config::Instance().CurrentTool(tool);
		uncheckButtons();
	}
	else
		Application::Config::Instance().CurrentTool(Tool::None);
}

void MainWindow::uncheckButtons()
{
	Tool currentTool = Application::Config::Instance().CurrentTool();
	for (ToolMap::iterator it = _tools.begin(); it != _tools.end(); ++it)
	{
		if (it->first != currentTool)
		{
			it->second->setChecked(false);
		}
	}
}

void MainWindow::grabItem(QPoint const & pos)
{
	_graphTabs.currentGraphView()->grabItem(pos);
}

void MainWindow::pointItem(QList<QGraphicsItem*> const & item)
{
}

void MainWindow::updateGraphStatus()
{
	GraphView * graphView = _graphTabs.currentGraphView();
	if (graphView == nullptr)
		return;
	Graph * graph = graphView->getGraphImage()->getGraph();
	QString newStatus = Application::Config::Instance().GraphStatusString()
		.arg(graph->VertexNumber()).arg(graph->EdgeNumber());
	ui.graphTextStatus->setText(newStatus);
}

void MainWindow::clickVertex(int id)
{

}

void MainWindow::clickOrderDirected(bool val)
{
	bool checked = true;
	if (val)
	{
		ui.actionDirectedGraph->setChecked(true);
		ui.actionUndirectedGraph->setChecked(false);
		_graphTabs.currentGraphView()->makeDirected();
	}
	else if (!val && !ui.actionDirectedGraph->isChecked())
	{
		ui.actionDirectedGraph->setChecked(true);
	}
	else
	{
		checked = false;
	}
	Application::Config::Instance().SetGraphDirected(checked);
}

void MainWindow::clickOrderUndirected(bool val)
{
	bool checked = false;
	if (val)
	{
		//ui.actionUndirectedGraph->setChecked(true);
		//ui.actionDirectedGraph->setChecked(false);
		//_graphTabs->currentGraphView()->makeUndirected();
		////EdgeVector vectorToRemove = _graph.GetNeighbours();
		////_graphTabs->currentGraphView()->removeEdges(vectorToRemove);
		//std::for_each(vectorToRemove.begin(), vectorToRemove.end(), [&](Edge * edge)
		//{
		//	//_graph.RemoveEdge(edge);
		//});
	}
	else if (!val && !ui.actionUndirectedGraph->isChecked())
	{
		ui.actionUndirectedGraph->setChecked(true);
	}
	else
	{
		checked = true;
	}
	Application::Config::Instance().SetGraphDirected(checked);
}

void MainWindow::clickWeighted(bool val)
{
	bool checked = true;
	if (val)
	{
		ui.actionWeightedGraph->setChecked(true);
		ui.actionUnweightedGraph->setChecked(false);
	}
	else if (!val && !ui.actionWeightedGraph->isChecked())
	{
		ui.actionWeightedGraph->setChecked(true);
	}
	else
	{
		checked = false;
	}
	Application::Config::Instance().SetGraphWeighted(checked);
}

void MainWindow::clickUnweighted(bool val)
{
	bool checked = false;
	if (val)
	{
		ui.actionUnweightedGraph->setChecked(true);
		ui.actionWeightedGraph->setChecked(false);
	}
	else if (!val && !ui.actionUnweightedGraph->isChecked())
	{
		ui.actionUnweightedGraph->setChecked(true);
	}
	else
	{
		checked = true;
	}
	Application::Config::Instance().SetGraphWeighted(checked);
}
