#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _currentTool(Tool::None)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	ui.setupUi(this);

	createActions();

	_graphScene = new QGraphicsScene(this);
	_graphScene->setSceneRect(ui.graphView->pos().x(), ui.graphView->pos().y(),
		ui.graphView->width(), ui.graphView->height());
	ui.graphView->setScene(_graphScene);

	_tools[Tool::Vertex]	= ui.actionAddVertex;
	_tools[Tool::Edge]		= ui.actionAddEdge;

	ui.graphView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	ui.graphView->setAlignment(Qt::AlignCenter);
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
	ui.actionAddVertex->setCheckable(true);
	connect(ui.actionAddEdge, SIGNAL(triggered(bool)), this, SLOT(checkAddEdgeButton(bool)));
	ui.actionAddEdge->setCheckable(true);

	connect(ui.graphView, SIGNAL(clicked(QPoint, QList<QGraphicsItem*>)), this, SLOT(clickGraphView(QPoint, QList<QGraphicsItem*>)));
}

void MainWindow::checkButton(Tool tool, bool b)
{
	if (b)
	{
		_currentTool = tool;
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
	VertexImage * item = new VertexImage(Application::Config::Instance().DefaultVertexContext());
	item->setVertex(vertex);
	item->setPos(ui.graphView->mapToScene(position));
	item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	item->setZValue(VERTICE_Z_VALUE);
	_graphScene->addItem(item);
	_graphScene->update();
	_vertexMap[vertex->Id()] = item;
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
			img->select(false);
		}
	}
}

void MainWindow::addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord)
{
	Vertex * first = _graph.VertexNo(pair.first);
	Vertex * second = _graph.VertexNo(pair.second);
	Edge * edge = new Edge(first, second);
	_graph.AddEdge(edge);

	EdgeImage * item = new EdgeImage(_vertexMap[first->Id()], _vertexMap[second->Id()]);
	item->setEdge(edge);
	item->setFlag(QGraphicsItem::ItemIsMovable, false);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	item->setZValue(EDGE_Z_VALUE);
	_graphScene->addItem(item);
	_graphScene->update();
}

void MainWindow::clickGraphView(QPoint const & position, QList<QGraphicsItem*> const & item)
{
	switch (_currentTool)
	{
	case Tool::None:
		break;
	case Tool::Vertex:
		if (item.size() == 0)
			addVertex(position);
		return;
	case Tool::Edge:
		if (item.size() == 0)
			return;
		buildEdge(item.first());
		return;
	default:
		break;
	}
}

void MainWindow::clickVertex(int id)
{

}
