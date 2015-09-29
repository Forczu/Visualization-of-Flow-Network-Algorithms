#pragma once

#include "Vertex.h"
#include "Edge.h"
#include "Matrix.h"

#include <vector>
#include <memory>

class Graph
{
	typedef std::shared_ptr<Vertex>	VertexPtr;
	typedef std::shared_ptr<Edge>	EdgePtr;
	typedef std::vector<VertexPtr>	VertexVector;
	typedef std::vector<EdgePtr>	EdgeVector;
	typedef std::pair<VertexVector, EdgeVector>	GraphPair;

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

	Edge * AddEdge();
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

