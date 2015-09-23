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

	_tools[Tool::Vertice]	= ui.actionAddVertice;
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

void MainWindow::checkAddVerticeButton(bool b)
{
	checkButton(Tool::Vertice, b);
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

	connect(ui.actionAddVertice, SIGNAL(triggered(bool)), this, SLOT(checkAddVerticeButton(bool)));
	ui.actionAddVertice->setCheckable(true);
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

void MainWindow::addVertice(QPoint const & position)
{
	Vertice * vertice = _graph.AddVertice();
	VerticeImage * item = new VerticeImage(Application::Config::Instance().DefaultVerticeContext());
	item->setVertice(vertice);
	item->setPos(ui.graphView->mapToScene(position));
	item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	item->setZValue(VERTICE_Z_VALUE);
	_graphScene->addItem(item);
	_graphScene->update();
	_verticeMap[vertice->Id()] = item;
}

void MainWindow::buildEdge(QGraphicsItem * const item)
{
	static std::pair<int, int> pair;
	static std::pair<QPointF, QPointF> coord;
	static bool firstVerticeChecked = true;
	VerticeImage * img = dynamic_cast<VerticeImage*>(item);
	if (img != NULL)
	{
		// jeœli pierwszy wierzcho³ek
		if (firstVerticeChecked)
		{
			pair.first = img->getVertice()->Id();
			coord.first = img->pos();
			firstVerticeChecked = false;
		}
		else
		{
			pair.second = img->getVertice()->Id();
			coord.second = img->pos();
			firstVerticeChecked = true;
			addEdge(pair, coord);
			img->select(false);
		}
	}
}

void MainWindow::addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord)
{
	Vertice * first = _graph.VerticeNo(pair.first);
	Vertice * second = _graph.VerticeNo(pair.second);
	Edge * edge = new Edge(first, second);
	_graph.AddEdge(edge);

	EdgeImage * item = new EdgeImage(_verticeMap[first->Id()], _verticeMap[second->Id()]);
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
	case Tool::Vertice:
		if (item.size() == 0)
			addVertice(position);
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

void MainWindow::clickVertice(int id)
{

}
