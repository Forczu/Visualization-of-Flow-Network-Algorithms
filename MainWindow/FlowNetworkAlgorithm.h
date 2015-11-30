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
	virtual void makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork) = 0;
	virtual QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) = 0;
	virtual void increaseFlow(QList<EdgeImage*> const & path) = 0;
	void setCurrentMaxFlow(int flow);
	inline int getMaxFlow() const { return _currentMaxFlow; }
};
