#pragma once
#include "GraphImage.h"

class GraphSerializer;

class DirectedGraphImage : public GraphImage
{
public:
	DirectedGraphImage(GraphConfig * config, QGraphicsScene * scene);
	~DirectedGraphImage();

	void addEdge(int vertexId1, int vertexId2) override;

protected:
	void updateVerticesDegree(VertexImage * vertexFrom, VertexImage * vertexTo);
	EdgeImage * createFullEdgeImage(Edge * edge, EdgeType type, int weight = 0) override;
};

