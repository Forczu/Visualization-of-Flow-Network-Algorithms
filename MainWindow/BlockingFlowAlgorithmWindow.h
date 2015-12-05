#pragma once
#include "FlowNetworkAlgorithmWindow.h"

class BlockingFlowAlgorithmWindow : public FlowNetworkAlgorithmWindow
{
	bool _blockingFlowInProgress;
	FlowNetwork * _blockingFlow;
	int _blockingStep;
	int _currentCapacity;
	QList<EdgeImage*> _currentBlockingPath;
	std::vector<QList<EdgeImage*>> _blockingPaths;
	GraphView * blockingView;
public:
	BlockingFlowAlgorithmWindow(FlowNetwork * network, FlowNetworkAlgorithm * algorithm, QWidget *parent = 0);

	void makeNextStep() override;
	int createResidualNetwork() override;
	void findAugumentingPath() override;
	void createBlockingFlow();

protected:
	void pushBlockingSet(QList<EdgeImage*> const & path, int capacity);
	void copyResidualNetworkAsBlockingFlow() const;
	void findAugumentingPathInBlockingFlow();
	void increaseFlowInResidaulNetwork();
	void showEvent(QShowEvent * evt) Q_DECL_OVERRIDE;
};

