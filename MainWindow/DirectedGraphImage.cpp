#include "DirectedGraphImage.h"
#include "Graph.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "Edge.h"
#include "QGraphicsItem"
#include "StraightEdgeImage.h"
#include "BezierEdgeImage.h"
#include "AWeightedStrategyBase.h"

DirectedGraphImage::DirectedGraphImage(GraphConfig * config)
: GraphImage(config)
{
}


DirectedGraphImage::DirectedGraphImage(DirectedGraphImage const & graph)
: GraphImage(graph)
{
	GraphImage const * tmp1 = &graph;
	GraphImage * tmp2 = const_cast<GraphImage*>(tmp1);
	cloneEdges(*tmp2);
}

DirectedGraphImage::~DirectedGraphImage()
{
}

EdgeImage * DirectedGraphImage::addEdge(int vertexId1, int vertexId2, int weight, EdgeType type /*= EdgeType::StraightLine*/, int flow /*= 0*/, float scale /*= 1.0f*/)
{
	Edge * edge = _graph->addEdge(vertexId1, vertexId2);
	if (edge == nullptr)
		return nullptr;
	auto edgeImg = createFullEdgeImage(edge, type, weight, flow, scale);
	return edgeImg;
}

void DirectedGraphImage::updateVerticesDegree(VertexImage * vertexFrom, VertexImage * vertexTo)
{
	std::pair<int, int> degree = _graph->getDegree(vertexFrom->getVertex());
	vertexFrom->setToolTip(degree.first, degree.second);
	degree = _graph->getDegree(vertexTo->getVertex());
	vertexTo->setToolTip(degree.first, degree.second);
}

EdgeImage * DirectedGraphImage::createFullEdgeImage(Edge * edge, EdgeType type /*= EdgeType::StraightLine*/, int weight /*= 0*/, int flow /*= 0*/, float scale /*= 1.0f*/)
{
	EdgeImage * edgeImg = createEdgeImage(edge, type, weight, scale);
	if (edgeImg == nullptr)
		return edgeImg;
	edgeImg->createArrowHead();
	edgeImg->setFlow(flow);
	Edge * neighbor = _graph->getNeighborEdge(edge);
	if (neighbor != nullptr)
		correctNeighborEdges(edge, neighbor);
	updateVerticesDegree(edgeImg->VertexFrom(), edgeImg->VertexTo());
	return edgeImg;
}
