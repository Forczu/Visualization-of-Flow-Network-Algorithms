#include "Graph.h"


Graph::Graph()
{
	_graph = new GraphPair();
}


Graph::~Graph()
{
	delete _graph;
}

bool Graph::VerticeExists(short verticeId) const
{
	if (!_graph->first.empty())
	{
		for each (auto v in _graph->first)
		{
			if (v->Id() == verticeId)
			{
				return true;
			}
		}
	}
	return false;
}

void Graph::AddVertice(Vertice * const vertice)
{
	_graph->first.push_back(vertice);
}

Vertice * Graph::AddVertice()
{
	int id = SmallestMissingIndex();
	Vertice * vertice = new Vertice(id);
	AddVertice(vertice);
	return vertice;
}

Vertice * Graph::AddVertice(int n)
{
	if (VerticeExists(n))
		return VerticeNo(n);
	Vertice * vertice = new Vertice(n);
	AddVertice(vertice);
	return vertice;
}

Vertice * Graph::VerticeNo(short n) const
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

void Graph::AddEdge(Edge * const edge)
{
	_graph->second.push_back(edge);
}

Edge * Graph::AddEdge()
{
	return nullptr;
}

Matrix Graph::GetNeighborhoodMatrix() const
{
	short n = VerticeNumber();
	Matrix nMatrix(n, n, 0);
	for each (auto edge in _graph->second)
	{
		nMatrix.Set((*edge)()->first->Id() - 1, (*edge)()->second->Id() - 1, 1);
	}
	return nMatrix;
}

short Graph::VerticeNumber() const
{
	return _graph->first.size();
}

int Graph::SmallestMissingIndex()
{
	int index = 1;	// poszukiwany indeks
	VerticeVector const & vertice = _graph->first;
	if (vertice.size() == 0)
		return index;
	bool notFound = true;
	for (; index <= _graph->first.size(); ++index)
	{
		notFound = true;
		for (VerticeVector::const_iterator it = vertice.begin(); it != vertice.end(); ++it)
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
