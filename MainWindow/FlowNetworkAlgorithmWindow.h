#ifndef FLOWNETWORKALGORITHMWINDOW_H
#define FLOWNETWORKALGORITHMWINDOW_H

#include <QDialog>
#include "ui_FlowNetworkAlgorithmWindow.h"
#include "FlowNetwork.h"
#include "FlowNetworkAlgorithm.h"
#include "AlgorithmProgressInfo.h"

class FordFulkersonAlgorithm;
class DinicAlgorithm;

class FlowNetworkAlgorithmWindow : public QDialog
{
	Q_OBJECT
protected:
	FlowNetwork * _network;
	FlowNetwork * _residualNetwork;
	QPointer<FlowNetworkAlgorithm> _algorithm;
	AlgorithmProgressInfo _info;
	bool _finished;
	QTimer * _timer;
	QPointer<QGraphicsScene> _scene;
	bool _started;
	std::vector<int> _capacities;
	std::vector<QList<EdgeImage*>> _paths;

public:
	explicit FlowNetworkAlgorithmWindow(FlowNetwork * network,
		FlowNetworkAlgorithm * algorithm, QWidget *parent = 0);
	virtual ~FlowNetworkAlgorithmWindow();
	void setSceneForViews(QGraphicsScene * scene);
	void configureView(GraphView* view) const;
	void scaleViews();
	void setAlgorithm(FlowNetworkAlgorithm * algorithm);
	void seNetwork(FlowNetwork * network);
protected:
	virtual void showEvent(QShowEvent * evt) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent * evt) Q_DECL_OVERRIDE;
	static int getCurrentFlowToTarget(FlowNetwork * network);
	void createConnections() const;
	void updateConsole(QString const & message);
	void deleteDialog() const;
	void createScene();
	virtual int createResidualNetwork();
	virtual void findAugumentingPath();
	void finishAlgorithm();
	virtual void increaseFlow();
	virtual void checkAlgorithmEnd();
	void pushNewSet(QList<EdgeImage*> const & path, int capacity);
	void clearSets();

private slots:
	virtual void makeNextStep();
	void finish();
	void stopTimer() const;
	void saveResult();
signals:
	void endAlgorithm();
protected:
	Ui::FlowNetworkAlgorithmWindow ui;
	float _scaleFactor;
	float _dx;
	int _step;
};

#endif // FLOWNETWORKALGORITHMWINDOW_H
