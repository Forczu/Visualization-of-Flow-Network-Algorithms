#include "Graph.h"


Graph::Graph()
{
	_graph = new GraphPair();
}


Graph::~Graph()
{
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

void Graph::AddVertex(Vertex * const vertex)
{
	_graph->first.push_back(vertex);
}

Vertex * Graph::AddVertex()
{
	int id = SmallestMissingIndex();
	Vertex * vertex = new Vertex(id);
	AddVertex(vertex);
	return vertex;
}

Vertex * Graph::AddVertex(int n)
{
	if (VertexExists(n))
		return VertexNo(n);
	Vertex * vertice = new Vertex(n);
	AddVertex(vertice);
	return vertice;
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
	short n = VertexNumber();
	Matrix nMatrix(n, n, 0);
	for each (auto edge in _graph->second)
	{
		nMatrix.Set((*edge)()->first->Id() - 1, (*edge)()->second->Id() - 1, 1);
	}
	return nMatrix;
}

short Graph::VertexNumber() const
{
	return _graph->first.size();
}

int Graph::SmallestMissingIndex()
{
	int index = 1;	// poszukiwany indeks
	VertexVector const & vertice = _graph->first;
	if (vertice.size() == 0)
		return index;
	bool notFound = true;
	for (; index <= _graph->first.size(); ++index)
	{
		notFound = true;
		for (VertexVector::const_iterator it = vertice.begin(); it != vertice.end(); ++it)
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
