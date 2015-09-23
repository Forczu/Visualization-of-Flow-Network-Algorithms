#include "Edge.h"


Edge::Edge(Vertex * const u, Vertex * const v)
{
	_pair = std::make_pair(u, v);
}


Edge::~Edge()
{
}
