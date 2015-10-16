#pragma once
#include "GraphImage.h"

class UndirectedGraphImage : public GraphImage
{
public:
	UndirectedGraphImage(QGraphicsScene * scene);
	~UndirectedGraphImage();

	void addEdge(int vertexId1, int vertexId2, QPointF const & coord1, QPointF const & coord2) override;
};

