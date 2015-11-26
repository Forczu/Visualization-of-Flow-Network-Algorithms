#include "mainwindow.h"
#include "VertexImage.h"
#include "GraphShapeDialog.h"
#include "Config.h"
#include "Vertex.h"
#include "StraightEdgeImage.h"
#include "LoopEdgeImage.h"
#include "GraphView.h"
#include "GraphImage.h"
#include "GraphSerializer.h"
#include "Tool.h"
#include "VertexAddTool.h"
#include "EdgeAddTool.h"
#include "RemoveTool.h"
#include "PointTool.h"
#include "QAction"
#include "IAlgorithm.h"
#include "FlowNetworkAlgorithmWindow.h"
#include "FordFulkersonAlgorithm.h"
#include "UndirectedGraphImage.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
	_graphTabs = new GraphTabWidget(this);
	ui.mainLayout->addWidget(_graphTabs, 0, 1, 4, 2);
	ui.mainLayout->setColumnStretch(0, 1);
	_graphTabs->hide();
	createActions();
	createButtonVector();
	checkPointerButton(true);
}

void MainWindow::createButtonVector()
{
	_buttons.push_back(ui.actionAddVertex);
	_buttons.push_back(ui.actionAddEdge);
	_buttons.push_back(ui.actionGrab);
	_buttons.push_back(ui.actionSelect);
	_buttons.push_back(ui.actionPointer);
	_buttons.push_back(ui.actionRemove);
}

MainWindow::~MainWindow()
{
	delete _graphTabs;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
}

void MainWindow::newFile()
{
	int index = _graphTabs->count() + 1;
	CreateNewGraphDialog dialog(index);
	dialog.show();
	dialog.exec();
	if (dialog.getResult() != QDialog::Accepted)
		return;
	if (_graphTabs->isHidden())
		_graphTabs->show();
	GraphImage * graph = createGraph(dialog.getGraphFunc(), dialog.getEdgeStrategy());
	_graphTabs->addTab(graph, dialog.getName());
}

void MainWindow::open()
{
	std::string fileName;
#ifdef DEBUG
	fileName = "serialized_graph.xml";
#else
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open Graph File..."), QString(), tr("XML File (*.xml)")).toStdString();
#endif
	GraphSerializer serializer;
	GraphImage * graph = serializer.deserialize(fileName);
	if (_graphTabs->isHidden())
		_graphTabs->show();
	_graphTabs->addTab(graph);
}

void MainWindow::saveAs()
{
	if (_graphTabs->count() == 0)
		return;
	QString fileName;
#ifdef DEBUG
	fileName = "serialized_graph.xml";
#else
	fileName = QFileDialog::getSaveFileName(this,
		tr("Save Graph File..."), QString(), tr("XML File (*.xml)"));
#endif
	auto graph = _graphTabs->currentGraphView()->getGraphImage();
	GraphSerializer serializer;
	serializer.serialize(*graph, fileName.toStdString());
}

void MainWindow::close()
{
	delete _graphTabs;
	QApplication::quit();
}

void MainWindow::checkAddVertexButton(bool b)
{
	checkButton(&VertexAddTool::Instance(), ui.actionAddVertex, b);
	setCursorForWidget(_graphTabs->currentWidget(), Qt::ArrowCursor);
}

void MainWindow::setCursorForWidget(QWidget * widget, Qt::CursorShape shape)
{
	if (!widget)
		return;
	widget->setCursor(shape);
}

void MainWindow::checkAddEdgeButton(bool b)
{
	checkButton(&EdgeAddTool::Instance(), ui.actionAddEdge, b);
	GraphView * currentView = _graphTabs->currentGraphView();
	if (currentView == nullptr)
		return;
	currentView->setCursor(Qt::ArrowCursor);
	currentView->setEdgeFlag(EdgeFlag::Source);
}

void MainWindow::checkSelectionButton(bool b)
{
	checkButton(0, 0);
}

void MainWindow::checkPointerButton(bool b)
{
	checkButton(&PointTool::Instance(), ui.actionPointer, b);
	setCursorForWidget(_graphTabs->currentWidget(), Qt::ArrowCursor);
}

void MainWindow::checkRemoveButton(bool b)
{
	checkButton(&RemoveTool::Instance(), ui.actionRemove, b);
	setCursorForWidget(_graphTabs->currentWidget(), Qt::CrossCursor);
}

void MainWindow::openGraphShapeDialog()
{
	if (_graphTabs->isHidden())
		return;
	GraphConfig * config = _graphTabs->currentGraphView()->getGraphImage()->getConfig();
	GraphShapeDialog graphShapeDialog = GraphShapeDialog(config, this);
	graphShapeDialog.setModal(false);
	graphShapeDialog.exec();
}

void MainWindow::createActions()
{
	ui.actionClose->setShortcuts(QKeySequence::Close);
	ui.actionClose->setStatusTip(tr("Zamyka program"));

	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.algorithmList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(runAlgorithm(QListWidgetItem*)));

	connect(ui.actionShape, SIGNAL(triggered()), this, SLOT(openGraphShapeDialog()));

	connect(_graphTabs, SIGNAL(currentChanged(int)), this, SLOT(changeGraphInformation()));
	connect(_graphTabs, SIGNAL(graphChanged()), this, SLOT(updateGraphStatus()));

	connect(ui.actionAddVertex, SIGNAL(triggered(bool)), this, SLOT(checkAddVertexButton(bool)));
	connect(ui.actionAddEdge, SIGNAL(triggered(bool)), this, SLOT(checkAddEdgeButton(bool)));
	connect(ui.actionSelect, SIGNAL(triggered(bool)), this, SLOT(checkSelectionButton(bool)));
	connect(ui.actionPointer, SIGNAL(triggered(bool)), this, SLOT(checkPointerButton(bool)));
	connect(ui.actionRemove, SIGNAL(triggered(bool)), this, SLOT(checkRemoveButton(bool)));

	connect(ui.actionStraightLine, SIGNAL(triggered(bool)), this, SLOT(checkStraightLine(bool)));
	connect(ui.actionBezierLine, SIGNAL(triggered(bool)), this, SLOT(checkBezierCurve(bool)));
}

void MainWindow::checkButton(Tool * tool, QAction * action, bool b)
{
	if (!b) action->setChecked(true);
	Application::Config::Instance().changeCurrentTool(tool);
	uncheckButtons(action);
}

void MainWindow::uncheckButtons(QAction const * action)
{
	for (ActionVector::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
	{
		if (*it != action)
		{
			(*it)->setChecked(false);
		}
	}
}

void MainWindow::grabItem(QPoint const & pos)
{
	_graphTabs->currentGraphView()->grabItem(pos);
}

void MainWindow::pointItem(QList<QGraphicsItem*> const & item)
{
}

GraphImage * MainWindow::createGraph(GraphCreateFunc graphFunc, EdgeStrategyCreateFunc strategyFunc)
{
	GraphImage * graph;
	GraphConfig * config = new GraphConfig(
		Application::Config::Instance().DefaultVertexContext()->clone(),
		Application::Config::Instance().DefaultEdgeContext()->clone(),
		Application::Config::Instance().SelectedVertexContext()->clone(),
		Application::Config::Instance().SelectedEdgeContext()->clone());
	graph = graphFunc(config);
	graph->setWeightStrategy(strategyFunc());
	return graph;
}

void MainWindow::checkStraightLine(bool b)
{
	if (b)
	{
		ui.actionBezierLine->setChecked(false);
		Application::Config::Instance().CurrentEdgeType(EdgeType::StraightLine);
	}
	else
		ui.actionStraightLine->setChecked(true);
}

void MainWindow::checkBezierCurve(bool b)
{
	if (b)
	{
		ui.actionStraightLine->setChecked(false);
		Application::Config::Instance().CurrentEdgeType(EdgeType::BezierLine);
	}
	else
		ui.actionBezierLine->setChecked(true);
}

void MainWindow::runAlgorithm(QListWidgetItem * item)
{
	GraphImage * graph = _graphTabs->currentGraphView()->getGraphImage();
	QDialog * window = _algorithmInfo.getDialog(graph, item->text());
	window->show();
}

void MainWindow::updateGraphStatus()
{
	GraphView * graphView = _graphTabs->currentGraphView();
	if (graphView == nullptr)
		return;
	Graph * graph = graphView->getGraphImage()->getGraph();
	QString newStatus = Application::Config::Instance().GraphStatusString()
		.arg(graph->VertexNumber()).arg(graph->EdgeNumber());
	ui.graphTextStatus->setText(newStatus);
}

void MainWindow::changeGraphInformation()
{
	GraphView * graphView = _graphTabs->currentGraphView();
	if (graphView == nullptr)
		return;
	auto graph = graphView->getGraphImage();
	_algorithmInfo.changeState(graph);
	QStringList algorithms = _algorithmInfo.getAlgorithmList();
	ui.algorithmList->clear();
	ui.algorithmList->addItems(algorithms);
	updateGraphStatus();
}