#pragma once
#include "GraphImage.h"

class DirectedGraphImage : public GraphImage
{
public:
	DirectedGraphImage(GraphConfig * config, QGraphicsScene * scene);
	~DirectedGraphImage();

	void addEdge(int vertexId1, int vertexId2, QPointF const & coord1, QPointF const & coord2) override;

private:
	void updateVerticesDegree(int vertexId1, int vertexId2);
};

