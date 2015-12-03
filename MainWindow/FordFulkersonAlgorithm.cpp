#include "FordFulkersonAlgorithm.h"
#include "FlowNetwork.h"
#include "Typedefs.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "Strings.h"
#include "FlowNetworkAlgorithmWindow.h"

FordFulkersonAlgorithm * FordFulkersonAlgorithm::getInstance()
{
	return new FordFulkersonAlgorithm;
}

int FordFulkersonAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork)
{
	return FlowNetworkAlgorithm::makeResidualNetwork(network, residualNewtork);
}

QList<EdgeImage*> FordFulkersonAlgorithm::findAugumentingPath(FlowNetwork * residualNetwork, int & capacity)
{
	return FlowNetworkAlgorithm::findAugumentingPath(residualNetwork, capacity);
}

void FordFulkersonAlgorithm::increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase)
{
	FlowNetworkAlgorithm::increaseFlow(network, path, increase);
}

QString FordFulkersonAlgorithm::resaidualNetworkFinishedMessage(int value)
{
	return Strings::Instance().get(FLOW_NETWORK_RESIDUAL_CREATED);
}

QString FordFulkersonAlgorithm::augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity)
{
	QString numbers;
	numbers.push_back(QString::number(path.first()->VertexFrom()->getId()) + ' ');
	for (EdgeImage * edge : path)
	{
		edge->setSelected(true);
		numbers.push_back(QString::number(edge->VertexTo()->getId()) + ' ');
	}
	QString message = Strings::Instance().get(FLOW_NETWORK_AUGUMENTING_PATH_FOUND)
		.arg(numbers).arg(capacity);
	return message;
}

void FordFulkersonAlgorithm::addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source, QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices)
{
	if (edge->VertexFrom() == currentVertex &&
		edge->VertexTo() != source &&
		!visitedVertices.contains(edge->VertexTo()) &&
		!rejectedVertices.contains(edge->VertexTo()))
	{
		possibleEdges.push_back(edge);
	}
}

void FordFulkersonAlgorithm::acceptNextStep(FlowNetworkAlgorithmWindow * window)
{
	window->visitFordFulkersonNextStep(this);
}

void FordFulkersonAlgorithm::acceptFindAugumentingPath(FlowNetworkAlgorithmWindow * window)
{

}
