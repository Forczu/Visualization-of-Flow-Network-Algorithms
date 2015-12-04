#pragma once
#include "FlowNetworkAlgorithm.h"
#include "BlockingFlowAlgoritm.h"

class DinicAlgorithm : public FlowNetworkAlgorithm, public BlockingFlowAlgoritm
{
public:
	static DinicAlgorithm * getInstance();

	int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork) override;

	virtual QString resaidualNetworkFinishedMessage(int value) override;

	QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) override;
private:

	void addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source,
		QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices) override;
};

