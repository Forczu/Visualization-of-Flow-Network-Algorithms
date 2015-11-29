#ifndef FLOWNETWORKALGORITHMWINDOW_H
#define FLOWNETWORKALGORITHMWINDOW_H

#include <QDialog>
#include "ui_FlowNetworkAlgorithmWindow.h"
#include "FlowNetwork.h"
#include "FlowNetworkAlgorithm.h"
#include "AlgorithmProgressInfo.h"

class FlowNetworkAlgorithmWindow : public QDialog
{
	Q_OBJECT
	FlowNetwork * _network;
	FlowNetwork * residualNewtork;
	AlgorithmProgressInfo _info;
public:
	FlowNetworkAlgorithmWindow(FlowNetwork * network,
		FlowNetworkAlgorithm * algorithm, QWidget *parent = 0);
	~FlowNetworkAlgorithmWindow();

	void scaleViews();
	void setAlgorithm(FlowNetworkAlgorithm * algorithm);
	void seNetwork(FlowNetwork * network);
protected:
	void showEvent(QShowEvent * evt) Q_DECL_OVERRIDE;
private:
	void configureView(GraphView * view, QGraphicsScene * scene);
	void createConnections();
	void updateConsole(QString const & message);
private slots:
	void makeNextStep();
	void finish();
private:
	Ui::FlowNetworkAlgorithmWindow ui;
	FlowNetworkAlgorithm * _algorithm;
	float _scaleFactor;
	float _dx;
	int _step;
};

#endif // FLOWNETWORKALGORITHMWINDOW_H
