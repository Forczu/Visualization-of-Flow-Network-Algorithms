#pragma once
#include "IAlgorithm.h"
#include "FlowNetwork.h"

class FlowNetworkAlgorithm : public IAlgorithm
{
protected:
	FlowNetwork * _network;
public:
	FlowNetworkAlgorithm();
	void setNetwork(FlowNetwork * network);
	virtual FlowNetwork * makeResidualNetwork() = 0;
	virtual QList<int> findAugumentingPath() = 0;
	virtual void increaseFlow(QList<int> const & path) = 0;
};

