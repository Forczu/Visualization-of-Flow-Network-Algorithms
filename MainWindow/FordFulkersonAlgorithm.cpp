#include "FordFulkersonAlgorithm.h"
#include "FlowNetworkAlgorithmWindow.h"
#include "FlowNetwork.h"
#include "Typedefs.h"
#include "Graph.h"
#include "Edge.h"
#include "EdgeImage.h"
#include "Vertex.h"

FordFulkersonAlgorithm * FordFulkersonAlgorithm::getInstance()
{
	return new FordFulkersonAlgorithm;
}

void FordFulkersonAlgorithm::run(GraphImage * graph)
{
}

FlowNetwork * FordFulkersonAlgorithm::makeResidualNetwork()
{
	FlowNetwork * residualNewtork = _network->clone();
	EdgeImageMap edges = residualNewtork->getEdges();
	for (EdgeImageMap::iterator it = edges.begin(); it != edges.end(); ++it)
	{
		Edge * edge = (*it).second->getEdge();
		int capacity = edge->getCapacity();
		int flow = edge->getFlow();
		Vertex * vertexFrom = edge->VertexFrom();
		Vertex * vertexTo = edge->VertexTo();
		int residualCapacity = capacity - flow;
		residualNewtork->removeEdge((*it).second);
		if (flow != 0)
			residualNewtork->addEdge(vertexTo->Id(), vertexFrom->Id(), flow, EdgeType::StraightLine);
		if (capacity != 0)
			residualNewtork->addEdge(vertexFrom->Id(), vertexTo->Id(), capacity, EdgeType::StraightLine);
	}
	return residualNewtork;
}

QList<int> FordFulkersonAlgorithm::findAugumentingPath()
{
	return QList<int>();
}

void FordFulkersonAlgorithm::increaseFlow(QList<int> const & path)
{

}
