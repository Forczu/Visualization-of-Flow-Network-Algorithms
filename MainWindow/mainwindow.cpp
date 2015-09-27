#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _currentTool(Tool::None)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	ui.setupUi(this);

	createActions();

	_graphScene = new QGraphicsScene(this);
	ui.graphView->setScene(_graphScene);

	_tools[Tool::Vertex]		= ui.actionAddVertex;
	_tools[Tool::Edge]			= ui.actionAddEdge;
	_tools[Tool::Grab]			= ui.actionGrab;
	_tools[Tool::RubberBand]	= ui.actionSelect;
	_tools[Tool::Pointer]		= ui.actionPointer;

	ui.graphView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	ui.graphView->setAlignment(Qt::AlignCenter);

	ui.actionGrab->setChecked(true);
}

MainWindow::~MainWindow()
{
	delete _graphScene;
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
}

void MainWindow::checkAddEdgeButton(bool b)
{
	checkButton(Tool::Edge, b);
}

void MainWindow::checkGrabButton(bool b)
{
	checkButton(Tool::Grab, b);
}

void MainWindow::checkSelectionButton(bool b)
{
	checkButton(Tool::RubberBand, b);
}

void MainWindow::checkPointerButton(bool b)
{
	checkButton(Tool::Pointer, b);
}

void MainWindow::openGraphShapeDialog()
{
	_graphShapeDialog = new GraphShapeDialog(this);
	_graphShapeDialog->show();
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
	if (img != NULL)
	{
		// jeœli pierwszy wierzcho³ek
		if (firstVertexChecked)
		{
			pair.first = img->getVertex()->Id();
			coord.first = img->pos();
			firstVertexChecked = false;
		}
		else
		{
			pair.second = img->getVertex()->Id();
			coord.second = img->pos();
			firstVertexChecked = true;
			addEdge(pair, coord);
			img->setSelected(false);
		}
	}
}

void MainWindow::addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord)
{
	Vertex * first = _graph.VertexNo(pair.first);
	Vertex * second = _graph.VertexNo(pair.second);
	Edge * edge = new Edge(first, second);
	_graph.AddEdge(edge);
	ui.graphView->addEdgeImage(edge, pair, coord);
	updateGraphStatus();
}

void MainWindow::grabItem(QPoint const & pos, QList<QGraphicsItem*> const & item)
{
	ui.graphView->grabItem(pos, item);
}

void MainWindow::pointItem(QList<QGraphicsItem*> const & item)
{
	ui.graphView->pointItem(item);
}

void MainWindow::updateGraphStatus()
{
	QString newStatus = Application::Config::Instance().GraphStatusString()
		.arg(_graph.VertexNumber()).arg(_graph.EdgeNumber());
	ui.graphTextStatus->setText(newStatus);
}

void MainWindow::clickGraphView(QPoint const & position, QList<QGraphicsItem*> const & item)
{
	switch (_currentTool)
	{
	case Tool::Vertex:
		if (item.size() == 0)
			addVertex(position);
		return;
	case Tool::Edge:
		if (item.size() == 0)
			return;
		buildEdge(item.first());
		return;
	case Tool::Pointer:
		pointItem(item);
		return;
	case Tool::Grab:
		grabItem(position, item);
		return;
	case Tool::RubberBand:
		ui.graphView->startRubberBand(position);
		break;
	default: case Tool::None:
		break;
	}
}

void MainWindow::clickVertex(int id)
{

}
