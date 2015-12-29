#pragma once
#include "GraphImage.h"

class DirectedGraphImage : public GraphImage
{
public:
	explicit DirectedGraphImage(GraphConfig * config);
	DirectedGraphImage(DirectedGraphImage const & graph);
	virtual ~DirectedGraphImage();
	EdgeImage * addEdge(int vertexId1, int vertexId2, int weight, EdgeType type = EdgeType::StraightLine, int flow = 0, float scale = 1.0f) override;
	static GraphImage * getInstance(GraphConfig * config)
	{
		return new DirectedGraphImage(config);
	}
	GraphImage * clone() const override
	{
		return new DirectedGraphImage(*this);
	}

protected:
	void updateVerticesDegree(VertexImage * vertexFrom, VertexImage * vertexTo);
	EdgeImage * createFullEdgeImage(Edge * edge, EdgeType type = EdgeType::StraightLine, int weight = 0, int flow = 0, float scale = 1.0f) override;

};

