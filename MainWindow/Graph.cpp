#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "Matrix.h"

Graph::Graph()
{
	_graph = new GraphPair;
}


Graph::Graph(Graph const & other)
{
	_graph = new GraphPair;
	for (Vertex * vertex : other._graph->first)
	{
		_graph->first.push_back(vertex->clone());
	}
	for (Edge * edge : other._graph->second)
	{
		Edge * newEdge = addEdge(edge->VertexFrom()->Id(), edge->VertexTo()->Id());
		newEdge->setFlow(edge->getFlow());
		newEdge->setCapacity(edge->getCapacity());
	}
}

Graph::~Graph()
{
	for (VertexVector::iterator it = _graph->first.begin(); it != _graph->first.end(); ++it)
	{
		if (*it != NULL)
			delete *it;
	}
	for (EdgeVector::iterator it = _graph->second.begin(); it != _graph->second.end(); ++it)
	{
		if (*it != NULL)
			delete *it;
	}
	delete _graph;
}

bool Graph::VertexExists(short vertexId) const
{
	if (!_graph->first.empty())
	{
		for each (auto v in _graph->first)
		{
			if (v->Id() == vertexId)
			{
				return true;
			}
		}
	}
	return false;
}

Vertex * Graph::AddVertex()
{
	int id = SmallestMissingVertexIndex();
	Vertex * vertex = new Vertex(id);
	AddVertex(vertex);
	return vertex;
}

Vertex * Graph::AddVertex(int n)
{
	if (VertexExists(n))
		return VertexNo(n);
	Vertex * vertex = new Vertex(n);
	AddVertex(vertex);
	return vertex;
}

void Graph::AddVertex(Vertex * const vertex)
{
	_graph->first.push_back(vertex);
}

Vertex * Graph::VertexNo(short n) const
{
	if (!_graph->first.empty())
	{
		for each (auto v in _graph->first)
		{
			if (v->Id() == n)
			{
				return v;
			}
		}
	}
	return nullptr;
}

void Graph::addEdge(Edge * const edge)
{
	_graph->second.push_back(edge);
}

Edge * Graph::addEdge(int first, int second)
{
	if (EdgeExists(first, second))
		return nullptr;
	int id = SmallestMissingEdgeIndex();
	Vertex * from = VertexNo(first);
	Vertex * to = VertexNo(second);
	Edge * edge = new Edge(from, to);
	edge->Id(id);
	addEdge(edge);
	return edge;
}

Edge * Graph::AddEdgeSingle(int first, int second)
{
	if (EdgeExists(first, second))
		return nullptr;
	int id = SmallestMissingEdgeIndex();
	Vertex * from = VertexNo(first);
	Vertex * to = VertexNo(second);
	Edge * edge = new Edge(from, to);
	edge->Id(id);
	addEdge(edge);
	return edge;
}

void Graph::RemoveVertex(short n)
{
	VertexVector * v = &_graph->first;
	for (VertexVector::iterator it = v->begin(); it != v->end(); ++it)
	{
		if ((*it)->Id() == n)
		{
			RemoveNeighbourEdges(*it);
			delete *it;
			v->erase(it);
			break;
		}
	}
}

void Graph::RemoveVertex(Vertex * const vertex)
{
	VertexVector * v = &_graph->first;
	for (VertexVector::iterator it = v->begin(); it != v->end(); ++it)
	{
		if ((*it) == vertex)
		{
			RemoveNeighbourEdges(*it);
			delete *it;
			v->erase(it);
			break;
		}
	}
}

void Graph::RemoveEdge(Edge * const edge)
{
	EdgeVector * e = &_graph->second;
	for (EdgeVector::iterator it = e->begin(); it != e->end(); ++it)
	{
		if ((*it) == edge)
		{
			delete *it;
			e->erase(it);
			break;
		}
	}
}

void Graph::RemoveNeighbourEdges(Vertex * const vertex)
{
	EdgeVector * e = &_graph->second;
	for (EdgeVector::iterator it = e->begin(); it != e->end(); )
	{
		if ((*it)->VertexFrom() == vertex || (*it)->VertexTo() == vertex)
		{
			delete *it;
			it = e->erase(it);
		}
		else
			++it;
	}
}

Matrix Graph::GetNeighborhoodMatrix() const
{
	short n = VertexNumber();
	Matrix nMatrix(n, n, 0);
	for each (auto edge in _graph->second)
	{
		nMatrix.Set(edge->VertexFrom()->Id() - 1, edge->VertexFrom()->Id() - 1, 1);
	}
	return nMatrix;
}

Edge * Graph::GetNeighborEdge(Edge * const edge)
{
	Vertex * first = edge->VertexFrom();
	Vertex * second = edge->VertexTo();
	auto it = std::find_if(_graph->second.begin(), _graph->second.end(), [&](Edge * e) -> bool
	{
		if (e->VertexFrom() == second && e->VertexTo() == first)
			return true;
		return false;
	});
	if (it != _graph->second.end())
		return *it;
	return nullptr;
}

int Graph::SmallestMissingVertexIndex()
{
	int index = 1;	// poszukiwany indeks
	VertexVector const & vertices = _graph->first;
	if (vertices.size() == 0)
		return index;
	bool notFound = true;
	for (; index <= vertices.size(); ++index)
	{
		notFound = true;
		for (VertexVector::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			// jeœli liczba jest obecna wœród wierzcho³ków, szukaj dalej
			if (index == (*it)->Id())
			{
				notFound = false;
				break;
			}
		}
		// liczba nie jest obecna - zwróæ j¹
		if (notFound)
			return index;
	}
	return index;
}

int Graph::SmallestMissingEdgeIndex()
{
	int index = 1;
	EdgeVector const & edges = _graph->second;
	if (edges.size() == 0)
		return index;
	bool notFound = true;
	for (; index <= edges.size(); ++index)
	{
		notFound = true;
		for (EdgeVector::const_iterator it = edges.begin(); it != edges.end(); ++it)
		{
			if (index == (*it)->Id())
			{
				notFound = false;
				break;
			}
		}
		if (notFound)
			return index;
	}
	return index;
}

bool Graph::EdgeExists(int from, int to)
{
	Vertex * first = VertexNo(from);
	Vertex * second = VertexNo(to);
	auto it = std::find_if(_graph->second.begin(), _graph->second.end(), [&](Edge * e) -> bool
	{
		if (e->VertexFrom() == first && e->VertexTo() == second)
			return true;
		return false;
	});
	return it != _graph->second.end();
}

bool Graph::EdgeExistsSingle(int from, int to)
{
	Vertex * first = VertexNo(from);
	Vertex * second = VertexNo(to);
	auto it = std::find_if(_graph->second.begin(), _graph->second.end(), [&](Edge * e) -> bool
	{
		if (e->VertexFrom() == first && e->VertexTo() == second ||
			e->VertexFrom() == second && e->VertexTo() == first)
			return true;
		return false;
	});
	return it != _graph->second.end();
}

std::pair<int, int> Graph::getDegree(Vertex * vertex)
{
	std::pair<int, int> degree;
	int indgree = 0, outdegre = 0;
	for (Edge * edge : _graph->second)
	{
		if (edge->VertexFrom() == vertex)
			++outdegre;
		else if (edge->VertexTo() == vertex)
			++indgree;
	}
	degree.first = indgree;
	degree.second = outdegre;
	return degree;
}

EdgeVector Graph::GetNeighbours()
{
	EdgeVector result;
	for (EdgeVector::iterator it = _graph->second.begin(); it != _graph->second.end(); ++it)
	{
		for (EdgeVector::iterator n_it = it; n_it != _graph->second.end(); ++n_it)
		{
			if ((*it)->VertexFrom() == (*n_it)->VertexTo() && (*it)->VertexTo() == (*n_it)->VertexFrom())
			{
				result.push_back(*it);
				break;
			}
		}
	}
	return result;
}

