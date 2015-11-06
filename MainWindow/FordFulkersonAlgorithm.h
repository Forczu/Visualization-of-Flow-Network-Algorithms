#pragma once
#include "FlowNetworkAlgorithm.h"
#include "FlowNetworkAlgorithmWindow.h"

class FordFulkersonAlgorithm : public FlowNetworkAlgorithm
{
public:
	static FordFulkersonAlgorithm * getInstance();
	void run(GraphImage * graph) override;
	FlowNetwork * makeResidualNetwork(FlowNetwork * network) override;
	QList<int> findAugumentingPath() override;
	void increaseFlow(QList<int> const & path) override;
};

