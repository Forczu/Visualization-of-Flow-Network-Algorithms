#pragma once
#include "FlowNetworkAlgorithm.h"

class FordFulkersonAlgorithm : public FlowNetworkAlgorithm
{
public:
	static FordFulkersonAlgorithm * getInstance();
	void run(GraphImage * graph) override;
	FlowNetwork * makeResidualNetwork() override;
	QList<int> findAugumentingPath() override;
	void increaseFlow(QList<int> const & path) override;
};

