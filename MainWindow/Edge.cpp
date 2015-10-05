#include "Edge.h"


Edge::Edge(Vertex * const u, Vertex * const v, int weight) : _weight(weight)
{
	_pair = std::make_pair(u, v);
}


Edge::Edge()
{

}

Edge::~Edge()
{
}
