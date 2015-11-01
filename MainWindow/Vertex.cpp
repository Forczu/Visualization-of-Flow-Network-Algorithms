#include "Vertex.h"


Vertex::Vertex(short id)
{
	_id = id;
}


Vertex::Vertex(Vertex const & v)
{
	_id = v._id;
}

Vertex::~Vertex()
{
}
