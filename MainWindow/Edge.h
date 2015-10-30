#pragma once

#include <utility>

class Vertex;

/// <summary>
/// £uk grafu. Relacja binarna pomiêdzy wierzcho³kami.
/// </summary>
class Edge
{
	typedef std::pair<Vertex*, Vertex*> vPair;
	vPair _pair;
	int _weight;
	int _id;

public:
	Edge();
	Edge(Vertex * const u, Vertex * const v, int weight = 0);
	~Edge();

	inline Vertex * VertexFrom() const { return _pair.first; }
	inline Vertex * VertexTo() const { return _pair.second; }
	int getWeight() const { return _weight; }
	void setWeight(int val) { _weight = val; }
	inline int Id() const { return _id; }
	void Id(int val) { _id = val; }
};

