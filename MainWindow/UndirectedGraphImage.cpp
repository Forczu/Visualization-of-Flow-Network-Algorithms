#include "UndirectedGraphImage.h"
#include "Graph.h"
#include "VertexImage.h"

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
	updateVerticesDegree(vertexId1, vertexId2);
}

void UndirectedGraphImage::updateVerticesDegree(int vertexId1, int vertexId2)
{
	VertexImage * vertexFrom = _vertexMap[vertexId1];
	std::pair<int, int> degree = _graph->getDegree(vertexFrom->getVertex());
	vertexFrom->setToolTip(degree.first + degree.second);
	VertexImage * vertexTo = _vertexMap[vertexId2];
	degree = _graph->getDegree(vertexTo->getVertex());
	vertexTo->setToolTip(degree.first + degree.second);
}
