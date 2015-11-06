#include "DirectedGraphImage.h"
#include "Graph.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "Edge.h"
#include "QGraphicsItem"
#include "StraightEdgeImage.h"
#include "BezierEdgeImage.h"

DirectedGraphImage::DirectedGraphImage(GraphConfig * config)
: GraphImage(config)
{
}


DirectedGraphImage::DirectedGraphImage(DirectedGraphImage const & graph)
: GraphImage(graph)
{
	cloneEdges(graph);
}

DirectedGraphImage::~DirectedGraphImage()
{
}

EdgeImage * DirectedGraphImage::addEdge(int vertexId1, int vertexId2, int weight, EdgeType type, int flow /*= 0*/)
{
	Edge * edge = _graph->addEdge(vertexId1, vertexId2);
	if (edge == nullptr)
		return nullptr;
	return createFullEdgeImage(edge, type, weight, flow);
}

void DirectedGraphImage::updateVerticesDegree(VertexImage * vertexFrom, VertexImage * vertexTo)
{
	std::pair<int, int> degree = _graph->getDegree(vertexFrom->getVertex());
	vertexFrom->setToolTip(degree.first, degree.second);
	degree = _graph->getDegree(vertexTo->getVertex());
	vertexTo->setToolTip(degree.first, degree.second);
}

EdgeImage * DirectedGraphImage::createFullEdgeImage(Edge * edge, EdgeType type, int weight /*= 0*/, int flow /*= 0*/)
{
	EdgeImage * edgeImg = createEdgeImage(edge, type, weight);
	if (edgeImg == nullptr)
		return edgeImg;
	edgeImg->addArrowHead();
	edgeImg->setFlow(flow);
	Edge * neighbor = _graph->GetNeighborEdge(edge);
	if (neighbor != nullptr)
		correctNeighborEdges(edge, neighbor);
	updateVerticesDegree(edgeImg->VertexFrom(), edgeImg->VertexTo());
	return edgeImg;
}
