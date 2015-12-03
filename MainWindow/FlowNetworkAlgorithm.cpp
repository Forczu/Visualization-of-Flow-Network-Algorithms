#include "FlowNetworkAlgorithm.h"
#include "EdgeImage.h"
#include "VertexImage.h"

FlowNetworkAlgorithm::FlowNetworkAlgorithm() : _scaleFactor(0.0f), _currentMaxFlow(0)
{
}

int FlowNetworkAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork)
{
	// usuniêcie starych krawêdzi
	auto oldEdges = outResidaulNetwork->getEdges();
	for (auto it = oldEdges.begin(); it != oldEdges.end(); ++it)
	{
		outResidaulNetwork->removeEdge((*it).second);
	}
	// analiza krawêdzi i utworzenie sieci residualnej
	auto edges = network->getEdges();
	QList<EdgeImage*> visitedNeighbours;
	EdgeImage * edge, *neighbor;
	for (EdgeImageMap::iterator it = edges.begin(); it != edges.end(); ++it)
	{
		edge = (*it).second;
		int capacity = edge->getCapacity();
		int flow = edge->getFlow();
		int vertexFromId = edge->VertexFrom()->getId();
		int vertexToId = edge->VertexTo()->getId();
		int residualCapacity = capacity - flow;
		if (edge->hasNeighbor() && !visitedNeighbours.contains(neighbor = network->edgeAt(vertexToId, vertexFromId)))
		{
			visitedNeighbours.push_back(neighbor);
			visitedNeighbours.push_back(edge);
			int neighborFlow = neighbor->getFlow();
			int neighborCapacity = neighbor->getCapacity();
			residualCapacity = capacity - flow + neighborFlow;
			int neighborResidualCapacity = neighborCapacity - neighborFlow + flow;
			if (residualCapacity != 0)
				outResidaulNetwork->addEdge(vertexFromId, vertexToId, residualCapacity, EdgeType::StraightLine);
			if (neighborResidualCapacity != 0)
				outResidaulNetwork->addEdge(vertexToId, vertexFromId, neighborResidualCapacity, EdgeType::StraightLine);
		}
		else
		{
			if (flow != 0)
				outResidaulNetwork->addEdge(vertexToId, vertexFromId, flow, EdgeType::StraightLine);
			if (residualCapacity != 0)
				outResidaulNetwork->addEdge(vertexFromId, vertexToId, residualCapacity, EdgeType::StraightLine);
		}
	}
	return 0;
}

QList<EdgeImage*> FlowNetworkAlgorithm::findAugumentingPath(FlowNetwork * residualNetwork, int & capacity)
{
	QList<EdgeImage*> augumentingPath;
	int sourceId = residualNetwork->getSourceId();
	VertexImage * source = residualNetwork->vertexAt(sourceId);
	EdgeImageMap edges = residualNetwork->getEdges();
	bool augumentationExists = std::any_of(edges.begin(), edges.end(), [&](EdgeImagePair item)
	{
		EdgeImage * edge = item.second;
		if (edge->VertexFrom() == source)
			return true;
		return false;
	});
	if (!augumentationExists)
		return augumentingPath;
	srand(time(NULL));
	bool finished = false;
	VertexImage * currentVertex = source;
	VertexImage * target = residualNetwork->vertexAt(residualNetwork->getTargetId());
	QList<VertexImage*> visitedVertices;
	QList<VertexImage*> rejectedVertices;
	EdgeImage * lastEdge = nullptr;
	while (!finished)
	{
		QList<EdgeImage*> possibleEdges;
		for (auto item : edges)
		{
			EdgeImage * edge = item.second;
			addEdgeToPath(possibleEdges, edge, currentVertex, source, visitedVertices, rejectedVertices);
		}
		if (possibleEdges.empty())
		{
			if (lastEdge == nullptr)
			{
				capacity = 0;
				finished = true;
				return augumentingPath;
			}
			else
			{
				rejectedVertices.push_back(currentVertex);
				augumentingPath.pop_back();
				if (!augumentingPath.empty())
				{
					lastEdge = augumentingPath.last();
					currentVertex = lastEdge->VertexTo();
				}
				else
				{
					lastEdge = nullptr;
					currentVertex = source;
				}
				continue;
			}
		}
		EdgeImage * chosenEdge = possibleEdges.at(rand() % possibleEdges.size());
		VertexImage * nextVertex = chosenEdge->VertexTo();
		visitedVertices.push_back(nextVertex);
		augumentingPath.push_back(chosenEdge);
		currentVertex = nextVertex;
		lastEdge = chosenEdge;
		if (currentVertex == target)
			finished = true;
	}
	// znajdz najmniejsza wartoœæ
	auto it = std::min_element(augumentingPath.begin(), augumentingPath.end(), [&](EdgeImage * edge1, EdgeImage * edge2)
	{
		return edge1->getCapacity() < edge2->getCapacity();
	});
	_currentMaxFlow += (capacity = (*it)->getCapacity());
	return augumentingPath;
}

void FlowNetworkAlgorithm::increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase)
{
	int oldFlow;
	EdgeImage * networkEdge;
	for (EdgeImage * edge : path)
	{
		int vertexFromId = edge->VertexFrom()->getId();
		int vertexToId = edge->VertexTo()->getId();
		networkEdge = network->edgeAt(vertexFromId, vertexToId);
		// je¿eli krawêdŸ nie istnieje w prawdziwej sieci, nale¿y utworzyæ przep³yw zwrotny
		if (networkEdge == nullptr)
		{
			networkEdge = network->edgeAt(vertexToId, vertexFromId);
			oldFlow = networkEdge->getFlow();
			networkEdge->setFlow(oldFlow - increase);
		}
		// je¿eli krawêdŸ istnieje, ale posiada s¹siada, nale¿y zmniejszyæ jego przep³yw
		else if (networkEdge != nullptr && networkEdge->hasNeighbor())
		{
			oldFlow = networkEdge->getFlow();
			EdgeImage * neighbourEdge = network->edgeAt(edge->VertexTo()->getId(), edge->VertexFrom()->getId());
			int currentNeighborFlow = neighbourEdge->getFlow();
			networkEdge->setFlow(std::max(increase - currentNeighborFlow, 0));
			neighbourEdge->setFlow(std::max(currentNeighborFlow - increase, 0));
		}
		// istnieje, ale nie posiada s¹siada, zwyk³e zwiêkszenie
		else
		{
			oldFlow = networkEdge->getFlow();
			networkEdge->setFlow(oldFlow + increase);
		}
		edge->setSelected(false);
		networkEdge->setSelected(true);
	}
}

void FlowNetworkAlgorithm::setCurrentMaxFlow(int flow)
{
	_currentMaxFlow = flow;
}

bool FlowNetworkAlgorithm::checkExistingPathsFromSource(FlowNetwork * network)
{
	EdgeImage * networkEdge;
	VertexImage * source = network->getSource();
	bool pathExists = false;
	for (auto item : network->getEdges())
	{
		networkEdge = item.second;
		if (networkEdge->VertexFrom() == source)
			if (networkEdge->getFlow() == networkEdge->getCapacity())
				continue;
			else
			{
				pathExists = true;
				break;
			}
	}
	return pathExists;
}

bool FlowNetworkAlgorithm::checkExistingPathsToTarget(FlowNetwork * network)
{
	EdgeImage * networkEdge;
	VertexImage * target = network->getTarget();
	bool pathExists = false;
	for (auto item : network->getEdges())
	{
		networkEdge = item.second;
		if (networkEdge->VertexTo() == target)
		{
			if (networkEdge->getFlow() == networkEdge->getCapacity())
				continue;
			else
			{
				pathExists = true;
				break;
			}
		}
	}
	return pathExists;
}
