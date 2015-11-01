#include "Edge.h"
#include "Vertex.h"

Edge::Edge(Vertex * const u, Vertex * const v, int capacity, int flow)
: _capacity(capacity), _flow(flow)
{
	_pair = std::make_pair(u, v);
}


Edge::Edge()
{
}

Edge::~Edge()
{
}
