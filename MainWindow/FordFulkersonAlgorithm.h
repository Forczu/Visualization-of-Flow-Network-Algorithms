#pragma once
#include "FlowNetworkAlgorithm.h"

class FordFulkersonAlgorithm : public FlowNetworkAlgorithm
{
public:
	static FordFulkersonAlgorithm * getInstance();
	void run(GraphImage * graph) override;
	void makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork) override;
	QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) override;
	void increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase) override;
};

