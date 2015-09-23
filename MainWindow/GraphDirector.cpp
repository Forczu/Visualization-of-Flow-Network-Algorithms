#include "GraphDirector.h"


GraphDirector::GraphDirector()
{
}


GraphDirector::~GraphDirector()
{
}

void GraphDirector::CreateGraph(GraphBuilder & builder) const
{
	builder.BuildGraph();
	builder.BuildVertice(1);
	builder.BuildVertice(2);
	builder.BuildVertice(3);
	builder.BuildEdge(1, 2);
	builder.BuildEdge(1, 3);
	builder.BuildEdge(2, 1);
	builder.BuildEdge(3, 1);
}
