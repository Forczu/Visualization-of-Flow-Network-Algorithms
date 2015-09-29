#pragma once

#include "Vertex.h"
#include "Edge.h"
#include "Matrix.h"
#include "Typedefs.h"

#include <vector>
#include <memory>

class Graph
{
	GraphPair * _graph;

	short _source = 0;
	short _target = 0;

public:
	Graph();
	~Graph();

	bool VertexExists(short verticeId) const;
	VertexPtr AddVertex();
	VertexPtr AddVertex(int n);
	void AddVertex(Vertex * const vertex);
	void AddVertex(VertexPtr const & vertex);
	VertexPtr VertexNo(short n) const;
	void RemoveVertex(short n);
	void RemoveVertex(Vertex * const vertex);

	void AddEdge(EdgePtr const & edge);
	void AddEdge(Edge * const edge);
	void RemoveEdge(Edge * const edge);
	void RemoveNeighbourEdges(VertexPtr const & vertex);

	Matrix GetNeighborhoodMatrix() const;

	inline short VertexNumber() const
	{
		return _graph->first.size();
	}
	inline short EdgeNumber() const
	{
		return _graph->second.size();
	}

	void SetSource(short vertice);
	void SetTarget(short vertice);

private:
	int SmallestMissingIndex();
};

