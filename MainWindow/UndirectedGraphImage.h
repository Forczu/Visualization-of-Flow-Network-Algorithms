#pragma once
#include "GraphImage.h"

class UndirectedGraphImage : public GraphImage
{
public:
	UndirectedGraphImage(GraphConfig * config, QGraphicsScene * scene);
	~UndirectedGraphImage();

	void addEdge(int vertexId1, int vertexId2) override;

protected:
	void updateVerticesDegree(VertexImage * vertexFrom, VertexImage * vertexTo);
	EdgeImage * createFullEdgeImage(Edge * edge, EdgeType type, int weight = 0) override;
};

