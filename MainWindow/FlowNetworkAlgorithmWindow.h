#ifndef FLOWNETWORKALGORITHMWINDOW_H
#define FLOWNETWORKALGORITHMWINDOW_H

#include <QDialog>
#include "ui_FlowNetworkAlgorithmWindow.h"
#include "FlowNetwork.h"
#include "FlowNetworkAlgorithm.h"

class FlowNetworkAlgorithmWindow : public QDialog
{
	Q_OBJECT
	QGraphicsScene * _residualScene;
	FlowNetwork * _network;
public:
	FlowNetworkAlgorithmWindow(QWidget *parent = 0);
	~FlowNetworkAlgorithmWindow();

	void scaleViews(FlowNetwork * network);
	void setAlgorithm(FlowNetworkAlgorithm * algorithm);
private:
	void createConnections();
private slots:
	void makeNextStep();
	void finish();
private:
	Ui::FlowNetworkAlgorithmWindow ui;
	FlowNetworkAlgorithm * _algorithm;
	float _scaleFactor;
	float _dx;
};

#endif // FLOWNETWORKALGORITHMWINDOW_H
