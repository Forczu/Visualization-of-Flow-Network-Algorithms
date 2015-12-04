#include "MkmAlgorithm.h"
#include "VertexImage.h"

MkmAlgorithm * MkmAlgorithm::getInstance()
{
	return new MkmAlgorithm;
}

int MkmAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork)
{
	showHiddenVertices();
	FlowNetworkAlgorithm::makeResidualNetwork(network, residualNewtork);
	int result = removeRedundantElements(residualNewtork);
	calculateVertexPotentials();
	return result;
}

QList<EdgeImage*> MkmAlgorithm::findAugumentingPath(FlowNetwork * residualNetwork, int & capacity)
{
	QList<EdgeImage*> path;
	return path;
}

void MkmAlgorithm::calculateVertexPotentials()
{
	
}

QString MkmAlgorithm::resaidualNetworkFinishedMessage(int value /*= 0*/)
{
	return BlockingFlowAlgoritm::getResidualNetworkFinishedMessage(value);
}

QString MkmAlgorithm::augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity)
{
	return BlockingFlowAlgoritm::getAugumentingPathFoundMessage(path, capacity);
}

void MkmAlgorithm::addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source, QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices)
{
	throw std::logic_error("The method or operation is not implemented.");
}
