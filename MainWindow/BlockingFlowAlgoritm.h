#pragma once
#include <QList>
#include <vector>

class EdgeImage;
class FlowNetwork;
class VertexImage;

class BlockingFlowAlgoritm
{
protected:
	std::vector<std::vector<float>> _pathMatrix;
	QList<VertexImage*> _currentHiddenVertices;
	int _sourceId;
	int _targetId;

	void hideRedundantVertices(FlowNetwork *& residualNewtork);

	void showHiddenVertices();

	void createShortestPathsMatrix(FlowNetwork *& residualNewtork);

	void removeRedundantEdges(FlowNetwork *& residualNewtork);

	int removeRedundantElements(FlowNetwork *& residualNewtork);

	QString getAugumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) const;

	QString getResidualNetworkFinishedMessage(int value) const;

};

