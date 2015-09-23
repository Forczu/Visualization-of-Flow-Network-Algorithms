#pragma once

#include "Graph.h"

class GraphBuilder
{
	Graph * _currentGraph;

public:
	GraphBuilder();
	~GraphBuilder();

	void BuildGraph();
	void BuildVertice();
	void BuildVertice(short n);
	void BuildEdge(short verticeFrom, short verticeTo);

	inline Graph * GetGraph() const
	{
		return _currentGraph;
	}
};

