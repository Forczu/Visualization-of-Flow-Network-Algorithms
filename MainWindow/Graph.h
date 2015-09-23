#pragma once

#include "Vertice.h"
#include "Edge.h"
#include "Matrix.h"

#include <vector>

class Graph
{
	typedef std::vector<Vertice*>					VerticeVector;
	typedef std::vector<Edge*>						EdgeVector;
	typedef std::pair<VerticeVector, EdgeVector>	GraphPair;

	GraphPair * _graph;

	short _source = 0;
	short _target = 0;

public:
	Graph();
	~Graph();

	bool VerticeExists(short verticeId) const;
	Vertice * AddVertice();
	Vertice * AddVertice(int n);
	void AddVertice(Vertice * const vertice);
	Vertice * VerticeNo(short n) const;

	Edge * AddEdge();
	void AddEdge(Edge * const edge);

	Matrix GetNeighborhoodMatrix() const;

	short VerticeNumber() const;

	void SetSource(short vertice);
	void SetTarget(short vertice);

private:
	int SmallestMissingIndex();
};

