#pragma once
#include <QMap>
#include "FlowNetworkAlgorithm.h"
#include "BlockingFlowAlgoritm.h"

class MkmAlgorithm : public FlowNetworkAlgorithm, public BlockingFlowAlgoritm
{
	QMap<VertexImage*, std::tuple<int, int, int>> _potentialMap;
public:
	static MkmAlgorithm * getInstance();

	int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork) override;

	QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) override;

	QString resaidualNetworkFinishedMessage(int value = 0) override;

	QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) override;

	void addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source, QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices) override;
private:
	void calculateVertexPotentials();
};

