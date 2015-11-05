#ifndef FLOWNETWORKALGORITHMWINDOW_H
#define FLOWNETWORKALGORITHMWINDOW_H

#include <QDialog>
#include "ui_FlowNetworkAlgorithmWindow.h"
#include "FlowNetwork.h"
#include "FlowNetworkAlgorithm.h"

class FlowNetworkAlgorithmWindow : public QDialog
{
	Q_OBJECT

public:
	FlowNetworkAlgorithmWindow(QWidget *parent = 0);
	~FlowNetworkAlgorithmWindow();

	void setMainNetwork(FlowNetwork * network);
	void setAlgorithm(FlowNetworkAlgorithm * algorithm);
private:
	void createConnections();
private slots:
	void makeNextStep();
	void finish();
private:
	Ui::FlowNetworkAlgorithmWindow ui;
	FlowNetwork * _mainNetwork;
	FlowNetworkAlgorithm * _algorithm;
};

#endif // FLOWNETWORKALGORITHMWINDOW_H
