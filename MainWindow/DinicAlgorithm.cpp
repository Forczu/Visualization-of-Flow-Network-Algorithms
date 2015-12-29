#include "DinicAlgorithm.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "FlowNetworkAlgorithmWindow.h"

namespace Dinic
{
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

	bool DinicAlgorithm::removeNeedlessElements(FlowNetwork * network)
	{
		bool verticesRemoved = false;
		VertexImage * source = network->getSource();
		VertexImage * target = network->getTarget();
		for (EdgeImage * edge : network->getEdges())
		{
			if (edge->getCapacity() == 0)
				network->removeEdge(edge);
		}
		for (auto vertex : network->getVertices())
		{
			if (vertex != source && vertex != target && (vertex->getOutDegree() == 0 || vertex->getInDegree() == 0))
			{
				vertex->hide();
				for (auto edge : network->getEdges())
				{
					if (edge->VertexFrom() == vertex || edge->VertexTo() == vertex)
						network->removeEdge(edge);
				}
				if (!verticesRemoved)
					verticesRemoved = true;
			}
		}
		return verticesRemoved;
	}

	QString DinicAlgorithm::resaidualNetworkFinishedMessage(int value)
	{
		return BlockingFlowAlgoritm::getResidualNetworkFinishedMessage(value);
	}

	QString DinicAlgorithm::augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity)
	{
		return BlockingFlowAlgoritm::getAugumentingPathFoundMessage(path, capacity);
	}
}