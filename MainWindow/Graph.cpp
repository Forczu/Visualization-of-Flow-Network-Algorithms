#include "Graph.h"
#include <algorithm>


Graph::Graph()
{
	_graph = new GraphPair;
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

VertexPtr Graph::AddVertex()
{
	int id = SmallestMissingIndex();
	VertexPtr vertex = VertexPtr(new Vertex(id));
	AddVertex(vertex);
	return vertex;
}

VertexPtr Graph::AddVertex(int n)
{
	if (VertexExists(n))
		return VertexNo(n);
	VertexPtr vertex = VertexPtr(new Vertex(n));
	AddVertex(vertex);
	return vertex;
}

void Graph::AddVertex(Vertex * const vertex)
{
	_graph->first.push_back(VertexPtr(vertex));
}

void Graph::AddVertex(VertexPtr const & vertex)
{
	_graph->first.push_back(vertex);
}

VertexPtr Graph::VertexNo(short n) const
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

void Graph::AddEdge(EdgePtr const & edge)
{
	_graph->second.push_back(edge);
}

void Graph::AddEdge(Edge * const edge)
{
	_graph->second.push_back(EdgePtr(edge));
}

void Graph::RemoveVertex(short n)
{
	VertexVector * v = &_graph->first;
	for (VertexVector::iterator it = v->begin(); it != v->end(); ++it)
	{
		if ((*it)->Id() == n)
		{
			RemoveNeighbourEdges(*it);
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
		if ((*it).get() == vertex)
		{
			RemoveNeighbourEdges(*it);
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
		if ((*it).get() == edge)
		{
			e->erase(it);
			break;
		}
	}
}

void Graph::RemoveNeighbourEdges(VertexPtr const & vertex)
{
	EdgeVector * e = &_graph->second;
	for (EdgeVector::iterator it = e->begin(); it != e->end(); )
	{
		if ((*it)->VertexFrom() == vertex.get() || (*it)->VertexTo() == vertex.get())
		{
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
