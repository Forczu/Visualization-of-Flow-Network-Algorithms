#include "MkmAlgorithm.h"
#include "VertexImage.h"
#include "EdgeImage.h"
#include "FlowNetwork.h"

MkmAlgorithm * MkmAlgorithm::getInstance()
{
	return new MkmAlgorithm;
}

int MkmAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork)
{
	showHiddenVertices();
	FlowNetworkAlgorithm::makeResidualNetwork(network, residualNewtork);
	int result = removeRedundantElements(residualNewtork);
	calculateVertexPotentials(network);
	auto vertex = findVertexWithMinimalPotential();
	vertex->setSelected(true);
	auto path1 = sendUnitsToTarget(network, vertex);
	auto path2 = sendUnitsToSource(network, vertex);
	return result;
}

QList<EdgeImage*> MkmAlgorithm::findAugumentingPath(FlowNetwork * residualNetwork, int & capacity)
{
	QList<EdgeImage*> path;
	return path;
}

/// <summary>
/// Oblicza 3 rodzaje potencja³ów dla ka¿dego werzcho³ka z sieci i zapisuje je do mapy.
/// </summary>
/// <param name="network">Sieæ przep³ywowa.</param>
void MkmAlgorithm::calculateVertexPotentials(FlowNetwork * network)
{
	VertexImage * source = network->getSource();
	VertexImage * target = network->getTarget();
	for (auto vertex : network->getVertices())
	{
		float inPotential, outPotential, potential;
		inPotential = outPotential = potential = 0;
		for (auto edge : network->getEdges())
		{
			if (edge->VertexFrom() == vertex)
			{
				outPotential += edge->getCapacity();
			}
			if (edge->VertexTo() == vertex)
			{
				inPotential += edge->getCapacity();
			}
		}
		if (vertex == source)
			inPotential = std::numeric_limits<float>::infinity();
		else if (vertex == target)
			outPotential = std::numeric_limits<float>::infinity();
		potential = std::min(inPotential, outPotential);
		_potentialMap[vertex] = std::make_tuple(inPotential, outPotential, potential);
	}
}

VertexImage * MkmAlgorithm::findVertexWithMinimalPotential()
{
	float minimalPotential = std::numeric_limits<float>::infinity();
	float currentPotential;
	VertexImage * chosenVertex = nullptr;
	for (PotentialMap::const_iterator it = _potentialMap.begin(); it != _potentialMap.end(); ++it)
	{
		currentPotential = std::get<2>(*it);
		if (currentPotential < minimalPotential)
		{
			chosenVertex = it.key();
			minimalPotential = currentPotential;
		}
	}
	return chosenVertex;
}

QList<EdgeImage*> MkmAlgorithm::sendUnitsToTarget(FlowNetwork * network, VertexImage * vertex)
{
	return findPathBetween(network, vertex, network->getTarget());
}

QList<EdgeImage*> MkmAlgorithm::sendUnitsToSource(FlowNetwork * network, VertexImage * vertex)
{
	return findPathBetween(network, network->getSource(), vertex);
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
	if (edge->VertexFrom() == currentVertex &&
		edge->VertexTo() != source &&
		!visitedVertices.contains(edge->VertexTo()) &&
		!rejectedVertices.contains(edge->VertexTo()))
	{
		possibleEdges.push_back(edge);
	}
}
