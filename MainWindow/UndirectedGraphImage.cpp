#include "UndirectedGraphImage.h"
#include "Graph.h"
#include "VertexImage.h"
#include "Edge.h"
#include "EdgeImage.h"

UndirectedGraphImage::UndirectedGraphImage(GraphConfig * config)
: GraphImage(config)
{
}


UndirectedGraphImage::~UndirectedGraphImage()
{
}

EdgeImage * UndirectedGraphImage::addEdge(int vertexId1, int vertexId2, int weight, EdgeType type)
{
	Edge * edge = _graph->AddEdgeSingle(vertexId1, vertexId2);
	if (edge == nullptr)
		return nullptr;
	return createFullEdgeImage(edge, Application::Config::Instance().CurrentEdgeType());
}

void UndirectedGraphImage::updateVerticesDegree(VertexImage * vertexFrom, VertexImage * vertexTo)
{
	std::pair<int, int> degree = _graph->getDegree(vertexFrom->getVertex());
	vertexFrom->setToolTip(degree.first + degree.second);
	degree = _graph->getDegree(vertexTo->getVertex());
	vertexTo->setToolTip(degree.first + degree.second);
}

EdgeImage * UndirectedGraphImage::createFullEdgeImage(Edge * edge, EdgeType type, int weight)
{
	EdgeImage * edgeImg = createEdgeImage(edge, type);
	if (edgeImg == nullptr)
		return edgeImg;
	edgeImg->setWeight(weight);
	updateVerticesDegree(edgeImg->VertexFrom(), edgeImg->VertexTo());
	return edgeImg;
}
