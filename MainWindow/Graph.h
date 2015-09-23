#pragma once

#include "Vertex.h"
#include "Edge.h"
#include "Matrix.h"

#include <vector>

class Graph
{
	typedef std::vector<Vertex*>					VertexVector;
	typedef std::vector<Edge*>						EdgeVector;
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
	void AddVertex(Vertex * const vertice);
	Vertex * VertexNo(short n) const;

	Edge * AddEdge();
	void AddEdge(Edge * const edge);

	Matrix GetNeighborhoodMatrix() const;

	short VertexNumber() const;

	void SetSource(short vertice);
	void SetTarget(short vertice);

private:
	int SmallestMissingIndex();
};

