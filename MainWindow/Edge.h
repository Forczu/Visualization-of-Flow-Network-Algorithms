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
	int _id;
	int _flow;
	int _capacity;

public:
	Edge();
	Edge(Vertex * const u, Vertex * const v, int capacity = 0, int flow = 0);
	~Edge();

	inline Vertex * VertexFrom() const { return _pair.first; }
	inline Vertex * VertexTo() const { return _pair.second; }
	int getCapacity() const { return _capacity; }
	void setCapacity(int val) { _capacity = val; }
	void setFlow(int val) { _flow = val; }
	int getFlow() const { return _flow; }
	inline int Id() const { return _id; }
	void Id(int val) { _id = val; }
};

