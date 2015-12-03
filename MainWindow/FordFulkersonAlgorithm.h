#pragma once
#include "FlowNetworkAlgorithm.h"

class FordFulkersonAlgorithm : public FlowNetworkAlgorithm
{
public:
	static FordFulkersonAlgorithm * getInstance();
	int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork) override;
	QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) override;
	void increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase) override;
	QString resaidualNetworkFinishedMessage(int param) override;
	QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) override;
	void addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source,
		QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices) override;

	void acceptNextStep(FlowNetworkAlgorithmWindow * window) override;
	void acceptFindAugumentingPath(FlowNetworkAlgorithmWindow * window) override;
};

