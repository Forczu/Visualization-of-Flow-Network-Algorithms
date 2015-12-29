#include "MkmAlgorithm.h"
#include "VertexImage.h"
#include "EdgeImage.h"
#include "FlowNetwork.h"

namespace Mkm
{
	int MkmAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork)
	{
		showHiddenVertices();
		FlowNetworkAlgorithm::makeResidualNetwork(network, residualNewtork);
		int result = removeRedundantElements(residualNewtork);
		calculateVertexPotentials(residualNewtork);
		return result;
	}

	QList<EdgeImage*> MkmAlgorithm::findAugumentingPath(FlowNetwork * network, int & capacity)
	{
		QList<EdgeImage*> pathToTarget, pathToSource;
		do
		{
			pathToTarget.clear();
			pathToSource.clear();
			VertexImage * vertex = findVertexWithMinimalPotential(network);
			if (vertex == nullptr)
				break;
			pathToTarget = sendUnitsToTarget(network, vertex->getId());
			pathToSource = sendUnitsToSource(network, vertex->getId());
			bool succeeded = pathToSource.size() != 0 && pathToTarget.size() != 0;
			if (!succeeded)
			{
				_rejectedVertices.push_back(vertex);
			}
			else
			{
				capacity = std::get<2>(_potentialMap[vertex->getId()]);
				_currentMaxFlow += capacity;
				vertex->setSelected(true);
				network->vertexAt(vertex->getId())->setSelected(true);
			}
		} while (pathToSource.size() == 0 || pathToTarget.size() == 0);
		return pathToSource + pathToTarget;
	}

	/// <summary>
	/// Oblicza 3 rodzaje potencja³ów dla ka¿dego werzcho³ka z sieci i zapisuje je do mapy.
	/// </summary>
	/// <param name="network">Sieæ przep³ywowa.</param>
	void MkmAlgorithm::calculateVertexPotentials(FlowNetwork * network)
	{
		_potentialMap.clear();
		_rejectedVertices.clear();
		VertexImage * source = network->getSource();
		VertexImage * target = network->getTarget();
		for (auto vertex : network->getVertices())
		{
			if (!vertex->isVisible())
				continue;
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
			_potentialMap[vertex->getId()] = std::make_tuple(inPotential, outPotential, potential);
		}
	}

	VertexImage * MkmAlgorithm::findVertexWithMinimalPotential(FlowNetwork * network)
	{
		float minimalPotential = std::numeric_limits<float>::infinity();
		VertexImage * chosenVertex = nullptr;
		for (PotentialMap::const_iterator it = _potentialMap.begin(); it != _potentialMap.end(); ++it)
		{
			float currentPotential = std::get<2>(*it);
			VertexImage * vertex = network->vertexAt(it.key());
			if (currentPotential != 0 && vertex->isVisible()
				&& !_rejectedVertices.contains(vertex) && currentPotential < minimalPotential)
			{
				chosenVertex = vertex;
				minimalPotential = currentPotential;
			}
		}
		return chosenVertex;
	}

	QList<EdgeImage*> MkmAlgorithm::sendUnitsToTarget(FlowNetwork * network, int vertexId)
	{
		return findPathBetween(network, network->vertexAt(vertexId), network->getTarget());
	}

	QList<EdgeImage*> MkmAlgorithm::sendUnitsToSource(FlowNetwork * network, int vertexId)
	{
		return findPathBetween(network, network->getSource(), network->vertexAt(vertexId));
	}

	bool MkmAlgorithm::removeNeedlessElements(FlowNetwork * network)
	{
		calculateVertexPotentials(network);
		bool removed = false;
		for (auto vertex : network->getVertices())
		{
			int vertexId = vertex->getId();
			if (std::get<0>(_potentialMap[vertexId]) == 0 || std::get<1>(_potentialMap[vertexId]) == 0)
			{
				vertex->hide();
				for (auto edge : network->getEdges())
				{
					if (edge->VertexFrom() == vertex || edge->VertexTo() == vertex)
						network->removeEdge(edge);
				}
				removed = true;
			}
		}
		return removed;
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
}