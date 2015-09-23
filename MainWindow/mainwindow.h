#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLayout>
#include <qtextcodec.h>

#include <map>

#include "ui_mainwindow.h"
#include "VerticeImage.h"
#include "Graph.h"
#include "GraphShapeDialog.h"
#include "Config.h"
#include "Tools.h"
#include "Vertice.h"
#include <vector>
#include "EdgeImage.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	static const int VERTICE_Z_VALUE = 3;
	static const int EDGE_Z_VALUE = 1;

	Graph _graph;

	typedef std::map<Tool, QAction*>  ToolMap;
	ToolMap _tools;

	QGraphicsScene * _graphScene;
	GraphShapeDialog * _graphShapeDialog;

	Tool _currentTool;

	std::map<int, VerticeImage*> _verticeMap;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;

protected:
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private slots:
	void close();

	void checkAddVerticeButton(bool b);
	void checkAddEdgeButton(bool b);

	void openGraphShapeDialog();

	void clickGraphView(QPoint const & pos, QList<QGraphicsItem*> const & item);
	void clickVertice(int id);

private:
	void createActions();
	void checkButton(Tool tool, bool b);
	void uncheckButtons();

	void addVertice(QPoint const & position);
	void buildEdge(QGraphicsItem * const item);
	void addEdge(std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord);
};

#endif // MAINWINDOW_H
