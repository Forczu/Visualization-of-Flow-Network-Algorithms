#pragma once

#include "Vertex.h"
#include <utility>

/// <summary>
/// £uk grafu. Relacja binarna pomiêdzy wierzcho³kami.
/// </summary>
class Edge
{
	typedef std::pair<Vertex*, Vertex*> vPair;

	vPair _pair;

	short _weight;

public:
	Edge(Vertex * const u, Vertex * const v);
	~Edge();

	inline Vertex * VertexFrom() const
	{
		return _pair.first;
	}
	inline Vertex * VertexTo() const
	{
		return _pair.second;
	}
};

