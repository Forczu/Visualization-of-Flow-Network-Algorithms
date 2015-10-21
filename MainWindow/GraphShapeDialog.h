#ifndef GRAPHSHAPEDIALOG_H
#define GRAPHSHAPEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include "ui_GraphShapeDialog.h"
#include "GraphConfig.h"

class ArrowHeadImage;
class Config;
class Edge;
class EdgeContext;
class StraightEdgeImage;
class Vertex;
class VertexImage;
class VertexContext;

class GraphShapeDialog : public QDialog
{
	Q_OBJECT
	GraphConfig * _config;

public:
	GraphShapeDialog(GraphConfig * config, QWidget *parent = 0);
	~GraphShapeDialog();

private:
	Ui::GraphShapeDialog ui;

	GraphConfig * _backupConfig;

	VertexImage * _verticePreview, *_v1, *_v2;
	StraightEdgeImage * _edgePreview;
	ArrowHeadImage * _arrow;
	Vertex * _vertex;
	Edge * _edge;

	void initVertexTab();
	void initEdgeTab();
	void clearPreview(QGraphicsView * const preview);

private slots:
	void vertexSizeChanged(int val);
	void strokeSizeChanged(int val);
	void vertexColorRedChanged(int val);
	void vertexColorGreenChanged(int val);
	void vertexColorBlueChanged(int val);
	void strokeColorRedChanged(int val);
	void strokeColorGreenChanged(int val);
	void strokeColorBlueChanged(int val);

	void edgeSizeChanged(int val);
	void edgeColorRedChanged(int val);
	void edgeColorGreenChanged(int val);
	void edgeColorBlueChanged(int val);

	void acceptChanges();
	void discardChanges();
	void applyChanges();

private:
	void updateVertexPreview();
	void updateEdgePreview();
	void createConnections();
	void backup();
	void enableApplyButton();
	void saveChanges();
	
};

#endif // GRAPHSHAPEDIALOG_H
