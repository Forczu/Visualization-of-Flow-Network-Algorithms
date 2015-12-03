#pragma once
#include "FlowNetworkAlgorithm.h"

class DinicAlgorithm : public FlowNetworkAlgorithm
{
	std::vector<std::vector<float>> _pathMatrix;
	QList<EdgeImage*> _usedEdges;
	QList<VertexImage*> _currentHiddenVertices;
	int _sourceId;
	int _targetId;
public:
	static DinicAlgorithm * getInstance();

	int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork) override;

	QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) override;

	void increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase) override;

	void run(GraphImage * graph) override;

	virtual QString resaidualNetworkFinishedMessage(int value) override;

	QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) override;
private:
	int removeRedundantElements(FlowNetwork *& residualNewtork);

	void removeRedundantEdges(FlowNetwork *& residualNewtork);

	void createShortestPathsMatrix(FlowNetwork *& residualNewtork);

	void hideRedundantVertices(FlowNetwork *& residualNewtork);

	void addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source,
		QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices) override;

};

