#include "FordFulkersonAlgorithm.h"
#include "FlowNetwork.h"
#include "Typedefs.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "Strings.h"

FordFulkersonAlgorithm * FordFulkersonAlgorithm::getInstance()
{
	return new FordFulkersonAlgorithm;
}

void FordFulkersonAlgorithm::run(GraphImage * graph)
{
}

int FordFulkersonAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork)
{
	return FlowNetworkAlgorithm::makeResidualNetwork(network, residualNewtork);
}

QList<EdgeImage*> FordFulkersonAlgorithm::findAugumentingPath(FlowNetwork * residualNetwork, int & capacity)
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
			if (edge->VertexFrom() == currentVertex &&
				edge->VertexTo() != source &&
				!visitedVertices.contains(edge->VertexTo()) &&
				!rejectedVertices.contains(edge->VertexTo()))
			{
				possibleEdges.push_back(edge);
			}
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
