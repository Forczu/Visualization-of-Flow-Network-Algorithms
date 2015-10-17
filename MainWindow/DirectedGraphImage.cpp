#include "DirectedGraphImage.h"
#include "Graph.h"
#include "EdgeImage.h"

DirectedGraphImage::DirectedGraphImage(QGraphicsScene * scene) : GraphImage(scene)
{

}

DirectedGraphImage::~DirectedGraphImage()
{
}

void DirectedGraphImage::addEdge(int vertexId1, int vertexId2, QPointF const & p1, QPointF const & p2)
{
	Edge * edge = _graph->AddEdge(vertexId1, vertexId2);
	if (edge == nullptr)
		return;
	EdgeImage * edgeImg = CreateEdgeImage(edge, p1, p2);
	if (edgeImg == nullptr)
		return;
	edgeImg->addArrowHead();
	Edge * neighbor = _graph->GetNeighborEdge(edge);
	if (neighbor != nullptr)
		correctNeighborEdges(edge, neighbor);
}
