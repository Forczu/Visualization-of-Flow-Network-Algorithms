#pragma once
#include "FlowNetworkAlgorithm.h"
#include "FlowNetworkAlgorithmWindow.h"

class FordFulkersonAlgorithm : public FlowNetworkAlgorithm
{
public:
	static FordFulkersonAlgorithm * getInstance();
	void run(GraphImage * graph) override;
	FlowNetwork * makeResidualNetwork(FlowNetwork * network) override;
	QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) override;
	void increaseFlow(QList<EdgeImage*> const & path) override;
};

