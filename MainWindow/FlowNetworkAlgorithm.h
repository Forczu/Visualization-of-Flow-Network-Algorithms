#pragma once
#include "IAlgorithm.h"
#include "FlowNetwork.h"

class FlowNetworkAlgorithm : public QObject, public IAlgorithm
{
	Q_OBJECT
protected:
	float _scaleFactor;
	int _currentMaxFlow;
	FlowNetworkAlgorithm();

public:
	virtual int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork);
	virtual QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) = 0;
	virtual void increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase);
	void setCurrentMaxFlow(int flow);
	inline int getMaxFlow() const { return _currentMaxFlow; }
	bool checkExistingPathsFromSource(FlowNetwork * network);
	bool checkExistingPathsToTarget(FlowNetwork * network);
	virtual QString resaidualNetworkFinishedMessage(int value = 0) = 0;
	virtual QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) = 0;
protected:

};
