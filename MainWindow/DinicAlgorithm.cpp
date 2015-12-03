#include "DinicAlgorithm.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "Strings.h"

DinicAlgorithm * DinicAlgorithm::getInstance()
{
	return new DinicAlgorithm;
}

int DinicAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork)
{
	for (auto vertex : _currentHiddenVertices)
		vertex->show();
	_currentHiddenVertices.clear();
	FlowNetworkAlgorithm::makeResidualNetwork(network, residualNewtork);
	int result = removeRedundantElements(residualNewtork);
	hideRedundantVertices(residualNewtork);
	return result;
}

QList<EdgeImage*> DinicAlgorithm::findAugumentingPath(FlowNetwork * residualNetwork, int & capacity)
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
				!rejectedVertices.contains(edge->VertexTo()) &&
				!_usedEdges.contains(edge))
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
	_usedEdges.append(augumentingPath);
	return augumentingPath;
}

void DinicAlgorithm::increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase)
{
	FlowNetworkAlgorithm::increaseFlow(network, path, increase);
}

void DinicAlgorithm::run(GraphImage * graph)
{
	
}

/// <summary>
/// Usuwa wszystkie nadmiarowe elementy grafu, które nie pojawi¹ siê w przep³ywie blokuj¹cym.
/// </summary>
/// <param name="residualNewtork">Sieæ przep³ywowa.</param>
/// <returns></returns>
int DinicAlgorithm::removeRedundantElements(FlowNetwork *& residualNewtork)
{
	_sourceId = residualNewtork->getSourceId() - 1;
	_targetId = residualNewtork->getTargetId() - 1;
	createShortestPathsMatrix(residualNewtork);
	hideRedundantVertices(residualNewtork);
	removeRedundantEdges(residualNewtork);
	float sourceTargetDistance = _pathMatrix[_sourceId][_targetId];
	return sourceTargetDistance == std::numeric_limits<float>::infinity() ? 0 : sourceTargetDistance;
}

/// <summary>
/// Usuwa nadmiarowe krawêdzie, które nie bior¹ udzia³u w tworzeniu przep³ywu blokuj¹cego.
/// </summary>
/// <param name="residualNewtork">Sieæ przep³ywowa.</param>
void DinicAlgorithm::removeRedundantEdges(FlowNetwork *& residualNewtork)
{
	QList<EdgeImage*> edgesToRemove;
	for (auto item : residualNewtork->getEdges())
	{
		EdgeImage * edge = item.second;
		int from = edge->VertexFrom()->getId() - 1;
		int to = edge->VertexTo()->getId() - 1;
		int first = _pathMatrix[_sourceId][to];
		int second = _pathMatrix[_sourceId][from];
		if (first <= second)
			edgesToRemove.push_back(edge);
	}
	for (auto edge : edgesToRemove)
	{
		residualNewtork->removeEdge(edge);
	}
	for (auto vertex : _currentHiddenVertices)
	{
		for (auto item : residualNewtork->getEdges())
		{
			EdgeImage * edge = item.second;
			if (edge->VertexFrom() == vertex || edge->VertexTo() == vertex)
			{
				residualNewtork->removeEdge(edge);
			}
		}
	}
}

/// <summary>
/// Tworzy macierz najkrótszych dróg z ka¿dego wierzcho³ka do innych.
/// D³ugoœci dróg obliczane s¹ algorytmem Floyda-Warshalla.
/// Poniewa¿ krawêdzi w sieci przep³ywowej posiadaj¹ przepustowoœæ i przep³yw,
/// zak³ada siê, ¿e ka¿da droga miêdzy s¹siednimi wierzcho³kami jest równa 1.
/// </summary>
/// <param name="newtork">Siec przep³ywowa.</param>
void DinicAlgorithm::createShortestPathsMatrix(FlowNetwork *& newtork)
{
	const int n = newtork->vertexNumber();
	_pathMatrix = std::vector<std::vector<float>>(n);
	for (int i = 0; i < n; ++i)
	{
		_pathMatrix[i].resize(n);
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (i != j)
				_pathMatrix[i][j] = std::numeric_limits<float>::infinity();
		}
	}
	std::pair<int, int> coord;
	for (auto item : newtork->getEdges())
	{
		coord = item.first;
		_pathMatrix[coord.first - 1][coord.second - 1] = 1.0f;
	}
	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
		{
			if (_pathMatrix[i][k] != std::numeric_limits<float>::infinity())
			{
				for (int j = 0; j < n; ++j)
				{
					float first = _pathMatrix[i][j];
					float second = _pathMatrix[i][k] + _pathMatrix[k][j];
					_pathMatrix[i][j] = std::min(first, second);
				}
			}
		}
	}
}

/// <summary>
/// Ukrywa nadmiarowe wierzcho³ki, które nie pojawi¹ siê w przep³ywie blokuj¹cym.
/// Przeszukiwanie przep³ywu odbywa siê poprzez krawêdzie, wiêc wierzcho³ki
/// wystarczy ukryæ, nie trzeba ich usuwaæ.
/// </summary>
/// <param name="residualNewtork">Sieæ przep³ywowa.</param>
void DinicAlgorithm::hideRedundantVertices(FlowNetwork *& residualNewtork)
{
	for (auto item : residualNewtork->getVertices())
	{
		VertexImage * vertex = item.second;
		int vertexId = vertex->getId() - 1;
		if (_pathMatrix[_sourceId][_targetId] <= _pathMatrix[_sourceId][vertexId] && vertexId != _targetId)
		{
			_currentHiddenVertices.push_back(vertex);
			vertex->hide();
		}
	}
}

QString DinicAlgorithm::resaidualNetworkFinishedMessage(int value)
{
	return Strings::Instance().get(LAYERED_RESIDUAL_NETWORK_CREATED).arg(value);
}

QString DinicAlgorithm::augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity)
{
	QString numbers;
	numbers.push_back(QString::number(path.first()->VertexFrom()->getId()) + ' ');
	for (EdgeImage * edge : path)
	{
		edge->setSelected(true);
		numbers.push_back(QString::number(edge->VertexTo()->getId()) + ' ');
	}
	QString message = Strings::Instance().get(BLOCKING_FLOW_AUGUMENTING_PATH)
		.arg(numbers).arg(capacity);
	return message;
}
