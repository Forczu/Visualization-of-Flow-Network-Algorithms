#include "GraphBuilder.h"


GraphBuilder::GraphBuilder()
{
	_currentGraph = nullptr;
}


GraphBuilder::~GraphBuilder()
{
}

void GraphBuilder::BuildGraph()
{
	_currentGraph = new Graph();
}

void GraphBuilder::BuildVertice()
{
	if (_currentGraph != nullptr)
	{
		
	}
}

void GraphBuilder::BuildVertice(short n)
{
	if (_currentGraph != nullptr && !_currentGraph->VerticeExists(n))
	{
		Vertice * vertice = new Vertice(n);
		_currentGraph->AddVertice(vertice);
	}
}

void GraphBuilder::BuildEdge(short verticeFrom, short verticeTo)
{
	if (_currentGraph != nullptr)
	{
		Vertice * vertice1 = _currentGraph->VerticeNo(verticeFrom);
		Vertice * vertice2 = _currentGraph->VerticeNo(verticeTo);
		Edge * edge = new Edge(vertice1, vertice2);
		_currentGraph->AddEdge(edge);
	}
}
