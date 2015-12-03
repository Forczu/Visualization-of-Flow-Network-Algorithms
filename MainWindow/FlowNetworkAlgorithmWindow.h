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
	FlowNetwork * _network;
	FlowNetwork * _residualNetwork;
	QPointer<FlowNetworkAlgorithm> _algorithm;
	AlgorithmProgressInfo _info;
	bool _finished;
	QTimer * _timer;
	QPointer<QGraphicsScene> _scene;
	bool _started;
	int _capacity;
	QList<EdgeImage*> _path;
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
	static int getCurrentFlowToTarget(FlowNetwork * network);
private:
	void configureView(GraphView * view) const;
	void createConnections() const;
	void updateConsole(QString const & message);
	void deleteDialog() const;
	void createScene();
	void createResidualNetwork();
	void findAugumentingPath();
	void finishAlgorithm();
	void increaseFlow();
	void checkAlgorithmEnd();

private slots:
	void makeNextStep();
	void finish();
	void stopTimer() const;
signals:
	void endAlgorithm();
private:
	Ui::FlowNetworkAlgorithmWindow ui;
	float _scaleFactor;
	float _dx;
	int _step;
};

#endif // FLOWNETWORKALGORITHMWINDOW_H
