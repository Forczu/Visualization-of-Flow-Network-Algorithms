#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLayout>
#include <QListWidget>
#include <qtextcodec.h>
#include <map>
#include <vector>
#include "ui_mainwindow.h"
#include "Graph.h"
#include "Typedefs.h"
#include "GraphTabWidget.h"
#include "CreateNewGraphDialog.h"
#include "Tool.h"
#include "AlgorithmInfo.h"

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

	typedef std::vector<QAction*> ActionVector;
	ActionVector _buttons;
	GraphTabWidget * _graphTabs;
	AlgorithmInfo _algorithmInfo;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;
	void setCursorForWidget(QWidget * widget, Qt::CursorShape shape);

protected:
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private slots:
	void newFile();
	void open();
	void saveAs();
	void close();
	void checkAddVertexButton(bool b);
	void checkAddEdgeButton(bool b);
	void checkSelectionButton(bool b);
	void checkPointerButton(bool b);
	void checkRemoveButton(bool b);
	void openGraphShapeDialog();
	void updateGraphStatus();
	void checkStraightLine(bool b);
	void checkBezierCurve(bool b);
	void runAlgorithm(QListWidgetItem * item);

private:
	void createActions();
	void createButtonVector();
	void checkButton(Tool * tool, QAction * action, bool b = true);
	void uncheckButtons(QAction const * action);

	void addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord);
	void grabItem(QPoint const & pos);
	void pointItem(QList<QGraphicsItem*> const & item);
	void removeItem(QList<QGraphicsItem*> const & item);
	void hideGraphTab();
};

#endif // MAINWINDOW_H
