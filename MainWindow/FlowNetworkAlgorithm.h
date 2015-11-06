#pragma once
#include "IAlgorithm.h"
#include "FlowNetwork.h"

class FlowNetworkAlgorithm : public IAlgorithm
{
protected:
	float _scaleFactor;
public:
	virtual FlowNetwork * makeResidualNetwork(FlowNetwork * network) = 0;
	virtual QList<int> findAugumentingPath() = 0;
	virtual void increaseFlow(QList<int> const & path) = 0;
};
