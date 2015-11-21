#pragma once
#include "GraphImage.h"

class UndirectedGraphImage : public GraphImage
{
public:
	UndirectedGraphImage(GraphConfig * config);
	UndirectedGraphImage(UndirectedGraphImage const & graph);
	~UndirectedGraphImage();

	EdgeImage * addEdge(int vertexId1, int vertexId2, int weight, EdgeType type, int flow = 0, float scale = 0.0f) override;
	static GraphImage * getInstance(GraphConfig * config)
	{
		return new UndirectedGraphImage(config);
	}

protected:
	void updateVerticesDegree(VertexImage * vertexFrom, VertexImage * vertexTo);
	EdgeImage * createFullEdgeImage(Edge * edge, EdgeType type, int weight = 0, int flow = 0) override;
};

