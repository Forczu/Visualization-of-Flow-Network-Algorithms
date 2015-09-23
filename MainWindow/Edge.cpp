#include "Edge.h"


Edge::Edge(Vertice * const u, Vertice * const v)
{
	_pair = std::make_pair(u, v);
}


Edge::~Edge()
{
}
