#include "FordFulkersonAlgorithm.h"
#include "FlowNetworkAlgorithmWindow.h"
#include "FlowNetwork.h"
#include "Typedefs.h"
#include "Graph.h"
#include "Edge.h"
#include "EdgeImage.h"
#include "Vertex.h"
#include "VertexImage.h"
#include "WeightedEdgeStrategy.h"

FordFulkersonAlgorithm * FordFulkersonAlgorithm::getInstance()
{
	return new FordFulkersonAlgorithm;
}

void FordFulkersonAlgorithm::run(GraphImage * graph)
{
}

FlowNetwork * FordFulkersonAlgorithm::makeResidualNetwork(FlowNetwork * network)
{
	FlowNetwork * residualNewtork = new FlowNetwork(network->getConfig()->clone());
	residualNewtork->setWeightStrategy(WeightedEdgeStrategy::getInstance());
	// skopiuj wierzcho³ki
	VertexImageMap vertices = network->getVertices();
	for (VertexImageMap::iterator it = vertices.begin(); it != vertices.end(); ++it)
	{
		VertexImage * vertex = (*it).second;
		int id = vertex->getId();
		QPointF position = vertex->scenePos();
		auto points = vertex->getPoints();
		residualNewtork->addVertex(id, position, points);
	}
	// analiza krawêdzi i utworzenie sieci residualnej
	EdgeImageMap edges = network->getEdges();
	for (EdgeImageMap::iterator it = edges.begin(); it != edges.end(); ++it)
	{
		Edge * edge = (*it).second->getEdge();
		int capacity = edge->getCapacity();
		int flow = edge->getFlow();
		Vertex * vertexFrom = edge->VertexFrom();
		Vertex * vertexTo = edge->VertexTo();
		int residualCapacity = capacity - flow;
		if (flow != 0)
			residualNewtork->addEdge(vertexTo->Id(), vertexFrom->Id(), flow, EdgeType::StraightLine);
		if (residualCapacity != 0)
			residualNewtork->addEdge(vertexFrom->Id(), vertexTo->Id(), residualCapacity, EdgeType::StraightLine);
	}
	residualNewtork->markSource(network->getSource());
	residualNewtork->markTarget(network->getTarget());
	return residualNewtork;
}

QList<int> FordFulkersonAlgorithm::findAugumentingPath()
{
	return QList<int>();
}

void FordFulkersonAlgorithm::increaseFlow(QList<int> const & path)
{

}
