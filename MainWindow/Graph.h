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

	short _source = 0;
	short _target = 0;

public:
	Graph();
	~Graph();

	bool VertexExists(short verticeId) const;
	Vertex * AddVertex();
	Vertex * AddVertex(int n);
	void AddVertex(Vertex * const vertex);
	Vertex * VertexNo(short n) const;
	void RemoveVertex(short n);
	void RemoveVertex(Vertex * const vertex);

	Edge * AddEdge(int first, int second);
	void AddEdge(Edge * const edge);
	Edge * AddEdgeSingle(int first, int second);
	void RemoveEdge(Edge * const edge);
	void RemoveNeighbourEdges(Vertex * const vertex);

	Matrix GetNeighborhoodMatrix() const;

	inline short VertexNumber() const
	{
		return _graph->first.size();
	}
	inline short EdgeNumber() const
	{
		return _graph->second.size();
	}

	void SetSource(short vertex);
	void SetTarget(short vertex);

	Edge * GetNeighborEdge(Edge * const edge);
	EdgeVector GetNeighbours();
	std::pair<int, int> getDegree(Vertex * vertex);

	inline VertexVector getVertices() const { return _graph->first; }
	inline EdgeVector getEdges() const { return _graph->second; }

private:
	int SmallestMissingVertexIndex();
	int SmallestMissingEdgeIndex();
	bool EdgeExists(int from, int to);
	bool EdgeExistsSingle(int from, int to);
};

