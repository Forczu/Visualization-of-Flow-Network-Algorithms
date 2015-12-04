#include "DinicAlgorithm.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "Strings.h"
#include "FlowNetworkAlgorithmWindow.h"

DinicAlgorithm * DinicAlgorithm::getInstance()
{
	return new DinicAlgorithm;
}

int DinicAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork)
{
	showHiddenVertices();
	FlowNetworkAlgorithm::makeResidualNetwork(network, residualNewtork);
	return removeRedundantElements(residualNewtork);
}

void DinicAlgorithm::addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source,
	QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices)
{
	if (edge->VertexFrom() == currentVertex &&
		edge->VertexTo() != source &&
		!visitedVertices.contains(edge->VertexTo()) &&
		!rejectedVertices.contains(edge->VertexTo()))
	{
		possibleEdges.push_back(edge);
	}
}

QString DinicAlgorithm::resaidualNetworkFinishedMessage(int value)
{
	return BlockingFlowAlgoritm::getResidualNetworkFinishedMessage(value);
}

QString DinicAlgorithm::augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity)
{
	return BlockingFlowAlgoritm::getAugumentingPathFoundMessage(path, capacity);
}
