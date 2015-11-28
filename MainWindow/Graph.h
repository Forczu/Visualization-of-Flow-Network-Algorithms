#pragma once

#include <algorithm>
#include <vector>
#include <memory>
#include "Typedefs.h"

class Vertex;
class Edge;
class Matrix;

class Graph
{
	GraphPair * _graph;

public:
	Graph();
	Graph(Graph const & other);
	~Graph();

	bool vertexExists(short verticeId) const;
	Vertex * addVertex();
	Vertex * addVertex(int n);
	void addVertex(Vertex * const vertex);
	Vertex * vertexNo(short n) const;
	void removeVertex(short n);
	void removeVertex(Vertex * const vertex);

	Edge * addEdge(int first, int second);
	void addEdge(Edge * const edge);
	Edge * addEdgeSingle(int first, int second);
	void removeEdge(Edge * const edge);
	void removeNeighbourEdges(Vertex * const vertex);

	Matrix getNeighborhoodMatrix() const;

	inline short VertexNumber() const
	{
		return _graph->first.size();
	}
	inline short EdgeNumber() const
	{
		return _graph->second.size();
	}

	Edge * getNeighborEdge(Edge * const edge);
	EdgeVector getNeighbours();
	std::pair<int, int> getDegree(Vertex * vertex);

	inline VertexVector getVertices() const { return _graph->first; }
	inline EdgeVector getEdges() const { return _graph->second; }

	inline Graph * clone() const { return new Graph(*this); }

	bool edgeExists(int from, int to);
	bool edgeExistsSingle(int from, int to);
private:
	int smallestMissingVertexIndex();
	int smallestMissingEdgeIndex();
};

