#pragma once

#include "Vertice.h"
#include <utility>

/// <summary>
/// £uk grafu. Relacja binarna pomiêdzy wierzcho³kami.
/// </summary>
class Edge
{
	typedef std::pair<Vertice*, Vertice*> vPair;

	vPair _pair;

	short _weight;

public:
	Edge(Vertice * const u, Vertice * const v);
	~Edge();

	vPair * operator()()
	{
		return &_pair;
	}
};

