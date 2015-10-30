#include "DirectedGraphImage.h"
#include "Graph.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "GraphSerializer.h"

DirectedGraphImage::DirectedGraphImage(GraphConfig * config, QGraphicsScene * scene)
: GraphImage(config, scene)
{
	GraphSerializer s;
	s.save(*this);
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
	updateVerticesDegree(vertexId1, vertexId2);
}

void DirectedGraphImage::updateVerticesDegree(int vertexId1, int vertexId2)
{
	VertexImage * vertexFrom = _vertexMap[vertexId1];
	std::pair<int, int> degree = _graph->getDegree(vertexFrom->getVertex());
	vertexFrom->setToolTip(degree.first, degree.second);
	VertexImage * vertexTo = _vertexMap[vertexId2];
	degree = _graph->getDegree(vertexTo->getVertex());
	vertexTo->setToolTip(degree.first, degree.second);
}
