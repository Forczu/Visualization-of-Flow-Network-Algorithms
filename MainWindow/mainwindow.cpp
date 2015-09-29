#include "mainwindow.h"

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
	VertexPtr vertex = _graph.AddVertex();
	ui.graphView->addVertexImage(vertex, position);
	updateGraphStatus();
}

void MainWindow::buildEdge(QGraphicsItem * const item)
{
	static std::pair<int, int> pair;
	static std::pair<QPointF, QPointF> coord;
	static bool firstVertexChecked = true;
	VertexImage * img = dynamic_cast<VertexImage*>(item);
	if (img != NULL)
	{
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
}

void MainWindow::addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord)
{
	Vertex * first = _graph.VertexNo(pair.first).get();
	Vertex * second = _graph.VertexNo(pair.second).get();
	Edge * edge = new Edge(first, second);
	_graph.AddEdge(edge);
	ui.graphView->addEdgeImage(edge, pair, coord);
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
				_graph.RemoveVertex(vImg->getVertex().get());
				ui.graphView->removeVertex(vImg);
				continue;
			}
			EdgeImage * eImg = dynamic_cast<EdgeImage*>(item);
			if (NULL != eImg)
			{
				_graph.RemoveEdge(eImg->getEdge());
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
