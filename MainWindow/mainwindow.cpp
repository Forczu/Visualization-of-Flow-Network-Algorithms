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

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
	_graphTabs = new GraphTabWidget(this);
	ui.mainLayout->addWidget(_graphTabs, 0, 1, 1, 2);
	ui.mainLayout->setColumnStretch(0, 1);
	_graphTabs->hide();
	createActions();

	_tools[Tool::Vertex]		= ui.actionAddVertex;
	_tools[Tool::Edge]			= ui.actionAddEdge;
	_tools[Tool::Grab]			= ui.actionGrab;
	_tools[Tool::RubberBand]	= ui.actionSelect;
	_tools[Tool::Pointer]		= ui.actionPointer;
	_tools[Tool::Remove]		= ui.actionRemove;

	ui.actionPointer->setChecked(true);
}

MainWindow::~MainWindow()
{
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
	if (!dialog.Confirmed())
		return;
	if (_graphTabs->isHidden())
		_graphTabs->show();
	_graphTabs->addTab(dialog.getName(), dialog.getOrder(), dialog.getWeighted());
}

void MainWindow::saveAs()
{
	if (_graphTabs->count() == 0)
		return;
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Open Graph File..."), QString(), tr("XML File (*.xml)"));
	auto graph = _graphTabs->currentGraphView()->getGraphImage();
	GraphSerializer serializer;
	serializer.save(*graph, fileName.toStdString());
}

void MainWindow::close()
{
	QApplication::quit();
}

void MainWindow::checkAddVertexButton(bool b)
{
	checkButton(Tool::Vertex, b);
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
	checkButton(Tool::Edge, b);
	GraphView * currentView = _graphTabs->currentGraphView();
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
	setCursorForWidget(_graphTabs->currentWidget(), Qt::OpenHandCursor);
}

void MainWindow::checkSelectionButton(bool b)
{
	checkButton(Tool::RubberBand, b);
}

void MainWindow::checkPointerButton(bool b)
{
	checkButton(Tool::Pointer, b);
	setCursorForWidget(_graphTabs->currentWidget(), Qt::ArrowCursor);
}

void MainWindow::checkRemoveButton(bool b)
{
	checkButton(Tool::Remove, b);
	setCursorForWidget(_graphTabs->currentWidget(), Qt::CrossCursor);
}

void MainWindow::openGraphShapeDialog()
{
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
	connect(ui.actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionShape, SIGNAL(triggered()), this, SLOT(openGraphShapeDialog()));

	connect(_graphTabs, SIGNAL(currentChanged(int)), this, SLOT(updateGraphStatus()));
	connect(_graphTabs, SIGNAL(graphChanged()), this, SLOT(updateGraphStatus()));

	connect(ui.actionAddVertex, SIGNAL(triggered(bool)), this, SLOT(checkAddVertexButton(bool)));
	connect(ui.actionAddEdge, SIGNAL(triggered(bool)), this, SLOT(checkAddEdgeButton(bool)));
	connect(ui.actionGrab, SIGNAL(triggered(bool)), this, SLOT(checkGrabButton(bool)));
	connect(ui.actionSelect, SIGNAL(triggered(bool)), this, SLOT(checkSelectionButton(bool)));
	connect(ui.actionPointer, SIGNAL(triggered(bool)), this, SLOT(checkPointerButton(bool)));
	connect(ui.actionRemove, SIGNAL(triggered(bool)), this, SLOT(checkRemoveButton(bool)));

	connect(ui.actionStraightLine, SIGNAL(triggered(bool)), this, SLOT(checkStraightLine(bool)));
	connect(ui.actionBezierLine, SIGNAL(triggered(bool)), this, SLOT(checkBezierCurve(bool)));
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
	_graphTabs->currentGraphView()->grabItem(pos);
}

void MainWindow::pointItem(QList<QGraphicsItem*> const & item)
{
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
