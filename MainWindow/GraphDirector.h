#pragma once

#include "GraphBuilder.h"

class GraphDirector
{
public:
	GraphDirector();
	~GraphDirector();

	void CreateGraph(GraphBuilder & builder) const;
};

