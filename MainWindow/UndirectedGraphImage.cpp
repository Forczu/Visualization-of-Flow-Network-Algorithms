#include "UndirectedGraphImage.h"
#include "Graph.h"

UndirectedGraphImage::UndirectedGraphImage(QGraphicsScene * scene) : GraphImage(scene)
{
}


UndirectedGraphImage::~UndirectedGraphImage()
{
}

void UndirectedGraphImage::addEdge(int vertexId1, int vertexId2, QPointF const & p1, QPointF const & p2)
{
	Edge * edge = _graph->AddEdgeSingle(vertexId1, vertexId2);
	if (edge == nullptr)
		return;
	CreateEdgeImage(edge, p1, p2);
}