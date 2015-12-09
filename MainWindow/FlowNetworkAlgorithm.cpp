#include "FlowNetworkAlgorithm.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "FlowNetwork.h"

FlowNetworkAlgorithm::FlowNetworkAlgorithm() : _scaleFactor(0.0f), _currentMaxFlow(0)
{
}

/// <summary>
/// Tworzy sieæ residualn¹ na podstawie przekazanej sieci przep³ywowej.
/// </summary>
/// <param name="network">The network.</param>
/// <param name="outResidaulNetwork">The residaul network.</param>
/// <returns></returns>
int FlowNetworkAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork)
{
	// usuniêcie starych krawêdzi
	auto oldEdges = outResidaulNetwork->getEdges();
	for (auto it = oldEdges.begin(); it != oldEdges.end(); ++it)
	{
		outResidaulNetwork->removeEdge(*it);
	}
	// analiza krawêdzi i utworzenie sieci residualnej
	auto edges = network->getEdges();
	QList<EdgeImage*> visitedNeighbours;
	EdgeImage *neighbor;
	for (EdgeImageMap::iterator it = edges.begin(); it != edges.end(); ++it)
	for (EdgeImage * edge : edges)
	{
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

/// <summary>
/// Wyszukuje œcie¿kê powiêkszaj¹c¹ w przekazanej sieci i zapisuje jej wartoœæ do capacity.
/// </summary>
/// <param name="network">The network.</param>
/// <param name="capacity">The capacity.</param>
/// <returns></returns>
QList<EdgeImage*> FlowNetworkAlgorithm::findAugumentingPath(FlowNetwork * network, int & capacity)
{
	QList<EdgeImage*> augumentingPath;
	VertexImage * source = network->getSource();
	VertexImage * target = network->getTarget();
	if (!checkAugumentingPathExists(network, source))
		return augumentingPath;
	augumentingPath = findPathBetween(network, source, target);
	if (augumentingPath.size() == 0)
	{
		capacity = 0;
	}
	else
	{
		// znajdz najmniejsza wartoœæ
		auto it = std::min_element(augumentingPath.begin(), augumentingPath.end(), [&](EdgeImage * edge1, EdgeImage * edge2)
		{
			return edge1->getCapacity() < edge2->getCapacity();
		});
		_currentMaxFlow += (capacity = (*it)->getCapacity());
	}
	return augumentingPath;
}

/// <summary>
/// Sprawdza czy istnieje œcie¿ka powiêkszaj¹ca prowadz¹ca ze Ÿród³a.
/// </summary>
/// <param name="network">The network.</param>
/// <param name="source">The source.</param>
/// <returns></returns>
bool FlowNetworkAlgorithm::checkAugumentingPathExists(FlowNetwork * network, VertexImage * source)
{
	EdgeImageMap edges = network->getEdges();
	return std::any_of(edges.begin(), edges.end(), [&](EdgeImage * edge)
	{
		if (edge->VertexFrom() == source)
			return true;
		return false;
	});
}

/// <summary>
/// Zwiêksza przep³yw w sieci w zadanej sciê¿ce o wartoœæ <param name="increase">The increase.</param>.
/// </summary>
/// <param name="network">The network.</param>
/// <param name="path">The path.</param>
/// <param name="increase">The increase.</param>
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

/// <summary>
/// Zwraca aktualnie maksymalny przep³yw.
/// </summary>
/// <param name="flow">The flow.</param>
void FlowNetworkAlgorithm::setCurrentMaxFlow(int flow)
{
	_currentMaxFlow = flow;
}

/// <summary>
/// Sprawdza czy istnieje œcie¿ka ze Ÿród³a w sieci.
/// </summary>
/// <param name="network">The network.</param>
/// <returns>true, je¿eli istnieje, przeciwnie false</returns>
bool FlowNetworkAlgorithm::checkExistingPathsFromSource(FlowNetwork * network)
{
	VertexImage * source = network->getSource();
	bool pathExists = false;
	for (auto networkEdge : network->getEdges())
	{
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

/// <summary>
/// Sprawdza czy istnieje œcie¿ka do ujœcia w sieci.
/// </summary>
/// <param name="network">The network.</param>
/// <returns>true, je¿eli istnieje, przeciwnie false</returns>
bool FlowNetworkAlgorithm::checkExistingPathsToTarget(FlowNetwork * network)
{
	VertexImage * target = network->getTarget();
	bool pathExists = false;
	for (auto networkEdge : network->getEdges())
	{
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

/// <summary>
/// Znajduje œcie¿kê w sieci pomiêdzy wybranymi wierzcho³kami.
/// </summary>
/// <param name="network">The network.</param>
/// <param name="from">Wierzcho³ek Ÿród³owy.</param>
/// <param name="to">Wierzcho³ek docelowy.</param>
/// <returns></returns>
QList<EdgeImage*> FlowNetworkAlgorithm::findPathBetween(FlowNetwork * network, VertexImage * from, VertexImage * to)
{
	QList<EdgeImage*> path;
	VertexImage * source = network->getSource();
	EdgeImageMap edges = network->getEdges();
	bool finished = false;
	VertexImage * currentVertex = from;
	QList<VertexImage*> visitedVertices;
	QList<VertexImage*> rejectedVertices;
	EdgeImage * lastEdge = nullptr;
	srand(time(NULL));
	while (!finished)
	{
		QList<EdgeImage*> possibleEdges;
		for (auto edge : edges)
		{
			addEdgeToPath(possibleEdges, edge, currentVertex, source, visitedVertices, rejectedVertices);
		}
		if (possibleEdges.empty())
		{
			if (lastEdge == nullptr)
			{
				finished = true;
				return path;
			}
			else
			{
				rejectedVertices.push_back(currentVertex);
				path.pop_back();
				if (!path.empty())
				{
					lastEdge = path.last();
					currentVertex = lastEdge->VertexTo();
				}
				else
				{
					lastEdge = nullptr;
					currentVertex = from;
				}
				continue;
			}
		}
		EdgeImage * chosenEdge = possibleEdges.at(rand() % possibleEdges.size());
		VertexImage * nextVertex = chosenEdge->VertexTo();
		visitedVertices.push_back(nextVertex);
		path.push_back(chosenEdge);
		currentVertex = nextVertex;
		lastEdge = chosenEdge;
		if (currentVertex == to)
			finished = true;
	}
	return path;
}

bool FlowNetworkAlgorithm::removeNeedlessElements(FlowNetwork * network)
{
	return false;
}
