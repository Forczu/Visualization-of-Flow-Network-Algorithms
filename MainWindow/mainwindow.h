#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLayout>
#include <qtextcodec.h>
#include <map>
#include <vector>
#include "ui_mainwindow.h"
#include "Graph.h"
#include "Typedefs.h"
#include "GraphTabWidget.h"
#include "CreateNewGraphDialog.h"

class Config;
class EdgeImage;
class GraphShapeDialog;
class GraphView;
class LoopEdgeImage;
class StraightEdgeImage;
class Vertex;
class VertexImage;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	typedef std::map<Tool, QAction*>  ToolMap;
	ToolMap _tools;

	GraphTabWidget _graphTabs;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;

protected:
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private slots:
	void newFile();
	void close();

	void checkAddVertexButton(bool b);
	void checkAddEdgeButton(bool b);
	void checkGrabButton(bool b);
	void checkSelectionButton(bool b);
	void checkPointerButton(bool b);
	void checkRemoveButton(bool b);

	void openGraphShapeDialog();
	void clickVertex(int id);

	void clickOrderDirected(bool val);
	void clickOrderUndirected(bool val);
	void clickWeighted(bool val);
	void clickUnweighted(bool val);

	void updateGraphStatus();

private:
	void createActions();
	void checkButton(Tool tool, bool b);
	void uncheckButtons();

	void addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord);
	void grabItem(QPoint const & pos);
	void pointItem(QList<QGraphicsItem*> const & item);
	void removeItem(QList<QGraphicsItem*> const & item);
};

#endif // MAINWINDOW_H
