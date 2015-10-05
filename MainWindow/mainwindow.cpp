#include "mainwindow.h"
#include "VertexImage.h"

#include "GraphShapeDialog.h"
#include "Config.h"
#include "Vertex.h"
#include "StraightEdgeImage.h"
#include "LoopEdgeImage.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _currentTool(Tool::None)
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

	ui.graphView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	ui.graphView->setAlignment(Qt::AlignCenter);

	ui.actionPointer->setChecked(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
}

void MainWindow::close()
{
	QApplication::quit();
}

void MainWindow::checkAddVertexButton(bool b)
{
	checkButton(Tool::Vertex, b);
	ui.graphView->setCursor(Qt::ArrowCursor);
}

void MainWindow::checkAddEdgeButton(bool b)
{
	checkButton(Tool::Edge, b);
	ui.graphView->setCursor(Qt::ArrowCursor);
	if (b)
		ui.graphView->setEdgeFlag(EdgeFlag::Source);
	else
		ui.graphView->setEdgeFlag(EdgeFlag::None);
	ui.graphView->AddEdgeFlag(b);
}

void MainWindow::checkGrabButton(bool b)
{
	checkButton(Tool::Grab, b);
	ui.graphView->setCursor(Qt::OpenHandCursor);
}

void MainWindow::checkSelectionButton(bool b)
{
	checkButton(Tool::RubberBand, b);
}

void MainWindow::checkPointerButton(bool b)
{
	checkButton(Tool::Pointer, b);
	ui.graphView->setCursor(Qt::ArrowCursor);
}

void MainWindow::checkRemoveButton(bool b)
{
	checkButton(Tool::Remove, b);
	ui.graphView->setCursor(Qt::CrossCursor);
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
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionShape, SIGNAL(triggered()), this, SLOT(openGraphShapeDialog()));

	connect(ui.actionAddVertex, SIGNAL(triggered(bool)), this, SLOT(checkAddVertexButton(bool)));
	connect(ui.actionAddEdge, SIGNAL(triggered(bool)), this, SLOT(checkAddEdgeButton(bool)));
	connect(ui.actionGrab, SIGNAL(triggered(bool)), this, SLOT(checkGrabButton(bool)));
	connect(ui.actionSelect, SIGNAL(triggered(bool)), this, SLOT(checkSelectionButton(bool)));
	connect(ui.actionPointer, SIGNAL(triggered(bool)), this, SLOT(checkPointerButton(bool)));
	connect(ui.actionRemove, SIGNAL(triggered(bool)), this, SLOT(checkRemoveButton(bool)));

	connect(ui.graphView, SIGNAL(clicked(QPoint, QList<QGraphicsItem*>)), this, SLOT(clickGraphView(QPoint, QList<QGraphicsItem*>)));

	connect(ui.actionDirectedGraph, SIGNAL(triggered(bool)), this, SLOT(clickOrderDirected(bool)));
	connect(ui.actionUndirectedGraph, SIGNAL(triggered(bool)), this, SLOT(clickOrderUndirected(bool)));
	connect(ui.actionWeightedGraph, SIGNAL(triggered(bool)), this, SLOT(clickWeighted(bool)));
	connect(ui.actionUnweightedGraph, SIGNAL(triggered(bool)), this, SLOT(clickUnweighted(bool)));
}

void MainWindow::checkButton(Tool tool, bool b)
{
	if (b)
	{
		_currentTool = tool;
		ui.graphView->setTool(_currentTool);
		uncheckButtons();
	}
	else
		_currentTool = Tool::None;
}

void MainWindow::uncheckButtons()
{
	for (ToolMap::iterator it = _tools.begin(); it != _tools.end(); ++it)
	{
		if (it->first != _currentTool)
		{
			it->second->setChecked(false);
		}
	}
}

void MainWindow::addVertex(QPoint const & position)
{
	Vertex * vertex = _graph.AddVertex();
	ui.graphView->addVertexImage(vertex, position);
	updateGraphStatus();
}

void MainWindow::buildEdge(QGraphicsItem * const item)
{
	static std::pair<int, int> pair;
	static std::pair<QPointF, QPointF> coord;
	static bool firstVertexChecked = true;
	VertexImage * img = dynamic_cast<VertexImage*>(item);
	if (img == NULL)
		return;
	// jeœli pierwszy wierzcho³ek
	if (firstVertexChecked)
	{
		pair.first = img->getVertex()->Id();
		coord.first = img->pos();
		ui.graphView->setEdgeFlag(EdgeFlag::Target);
		firstVertexChecked = false;
	}
	else
	{
		pair.second = img->getVertex()->Id();
		coord.second = img->pos();
		ui.graphView->setEdgeFlag(EdgeFlag::None);
		firstVertexChecked = true;
		addEdge(pair, coord);
	}
}

void MainWindow::addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord)
{
	Edge * edge = _graph.AddEdge(pair.first, pair.second);
	if (edge == nullptr)
		return;
	ui.graphView->addEdgeImage(edge, pair, coord);
	Edge * neighbor = _graph.GetNeighborEdge(edge);
	if (neighbor != nullptr)
		ui.graphView->correctNeighborEdges(edge, neighbor);
	updateGraphStatus();
}

void MainWindow::grabItem(QPoint const & pos)
{
	ui.graphView->grabItem(pos);
}

void MainWindow::pointItem(QList<QGraphicsItem*> const & item)
{
}

void MainWindow::updateGraphStatus()
{
	QString newStatus = Application::Config::Instance().GraphStatusString()
		.arg(_graph.VertexNumber()).arg(_graph.EdgeNumber());
	ui.graphTextStatus->setText(newStatus);
}

void MainWindow::removeItem(QList<QGraphicsItem*> const & items)
{
	for each (QGraphicsItem* item in items)
	{
		try
		{
			VertexImage * vImg = dynamic_cast<VertexImage*>(item);
			if (NULL != vImg)
			{
				Vertex * vertex = vImg->getVertex();
				_graph.RemoveVertex(vertex);
				ui.graphView->removeVertex(vImg);
				continue;
			}
			EdgeImage * eImg = dynamic_cast<EdgeImage*>(item);
			if (NULL != eImg)
			{
				Edge * edge = eImg->getEdge();
				_graph.RemoveEdge(edge);
				ui.graphView->removeEdge(eImg);
				continue;
			}
		}
		catch (std::exception & e)
		{
		}
	}
}

void MainWindow::clickGraphView(QPoint const & position, QList<QGraphicsItem*> const & items)
{
	switch (_currentTool)
	{
	case Tool::Vertex:
		if (items.size() == 0)
			addVertex(position);
		return;
	case Tool::Edge:
		if (items.size() == 0)
			return;
		buildEdge(items.first());
		return;
	case Tool::Pointer:
		ui.graphView->pointItem(position, items);
		return;
	case Tool::Grab:
		grabItem(position);
		return;
	case Tool::RubberBand:
		ui.graphView->startRubberBand(position);
		break;
	case Tool::Remove:
		if (items.size() != 0)
		{
			if (!items.first()->isSelected())
				removeItem(items);
			else
				removeItem(ui.graphView->scene()->selectedItems());
			updateGraphStatus();
		}
		return;
	default: case Tool::None:
		break;
	}
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
		ui.graphView->makeDirected();
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
		ui.actionUndirectedGraph->setChecked(true);
		ui.actionDirectedGraph->setChecked(false);
		ui.graphView->makeUndirected();
		EdgeVector vectorToRemove = _graph.GetNeighbours();
		ui.graphView->removeEdges(vectorToRemove);
		std::for_each(vectorToRemove.begin(), vectorToRemove.end(), [&](Edge * edge)
		{
			_graph.RemoveEdge(edge);
		});
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
