#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLayout>
#include <qtextcodec.h>

#include <map>

#include "ui_mainwindow.h"
#include "VertexImage.h"
#include "Graph.h"
#include "GraphShapeDialog.h"
#include "Config.h"
#include "Tools.h"
#include "Vertex.h"
#include <vector>
#include "StraightEdgeImage.h"
#include "LoopEdgeImage.h"
#include "Typedefs.h"

class VertexImage;
class EdgeImage;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Graph _graph;

	typedef std::map<Tool, QAction*>  ToolMap;
	ToolMap _tools;

	QGraphicsScene * _graphScene;

	Tool _currentTool;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;

protected:
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private slots:
	void close();

	void checkAddVertexButton(bool b);
	void checkAddEdgeButton(bool b);
	void checkGrabButton(bool b);
	void checkSelectionButton(bool b);
	void checkPointerButton(bool b);
	void checkRemoveButton(bool b);

	void openGraphShapeDialog();

	void clickGraphView(QPoint const & position, QList<QGraphicsItem*> const & item);
	void clickVertex(int id);

private:
	void createActions();
	void checkButton(Tool tool, bool b);
	void uncheckButtons();

	void addVertex(QPoint const & position);
	void buildEdge(QGraphicsItem * const item);
	void addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord);
	void grabItem(QPoint const & pos);
	void pointItem(QList<QGraphicsItem*> const & item);
	void updateGraphStatus();
	void removeItem(QList<QGraphicsItem*> const & item);
};

#endif // MAINWINDOW_H
