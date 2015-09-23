#ifndef GRAPHSHAPEDIALOG_H
#define GRAPHSHAPEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include "ui_GraphShapeDialog.h"
#include "VertexImage.h"
#include "Vertex.h"
#include "Config.h"
#include "TextItem.h"

class GraphShapeDialog : public QDialog
{
	Q_OBJECT

public:
	GraphShapeDialog(QWidget *parent = 0);
	~GraphShapeDialog();

private:
	Ui::GraphShapeDialog ui;

	QGraphicsScene * _graphScene;
	VertexContext * _currentContext;
	VertexImage * _verticePreview;

private slots:
	void verticeSizeChanged(int val);
	void strokeSizeChanged(int val);
	void verticeColorRedChanged(int val);
	void verticeColorGreenChanged(int val);
	void verticeColorBlueChanged(int val);
	void strokeColorRedChanged(int val);
	void strokeColorGreenChanged(int val);
	void strokeColorBlueChanged(int val);

	void acceptChanges();
	void discardChanges();
	void applyChanges();

#pragma region Funkcje pomocnicze
private:
	void updatePreview();
	void createConnections();
	void backup();
	void enableApplyButton();
#pragma endregion
	
};

#endif // GRAPHSHAPEDIALOG_H
