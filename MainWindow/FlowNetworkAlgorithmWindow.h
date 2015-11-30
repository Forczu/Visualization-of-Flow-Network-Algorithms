#ifndef FLOWNETWORKALGORITHMWINDOW_H
#define FLOWNETWORKALGORITHMWINDOW_H

#include <QDialog>
#include "ui_FlowNetworkAlgorithmWindow.h"
#include "FlowNetwork.h"
#include "FlowNetworkAlgorithm.h"
#include "AlgorithmProgressInfo.h"
#include <QPointer>

class FlowNetworkAlgorithmWindow : public QDialog
{
	Q_OBJECT
	QPointer<FlowNetwork> _network;
	QPointer<FlowNetwork> _residualNetwork;
	QPointer<FlowNetworkAlgorithm> _algorithm;
	AlgorithmProgressInfo _info;
	bool _finished;
	QTimer * _timer;
	QPointer<QGraphicsScene> _scene;
public:
	FlowNetworkAlgorithmWindow(FlowNetwork * network,
		FlowNetworkAlgorithm * algorithm, QWidget *parent = 0);
	~FlowNetworkAlgorithmWindow();
	void setSceneForViews(QGraphicsScene * scene);
	void scaleViews();
	void setAlgorithm(FlowNetworkAlgorithm * algorithm);
	void seNetwork(FlowNetwork * network);
protected:
	void showEvent(QShowEvent * evt) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent * evt) Q_DECL_OVERRIDE;
private:
	void configureView(GraphView * view);
	void createConnections();
	void updateConsole(QString const & message);
	void deleteDialog();
private slots:
	void makeNextStep();
	void finish();
	void stopTimer();
signals:
	void endAlgorithm();
private:
	Ui::FlowNetworkAlgorithmWindow ui;
	float _scaleFactor;
	float _dx;
	int _step;
};

#endif // FLOWNETWORKALGORITHMWINDOW_H
