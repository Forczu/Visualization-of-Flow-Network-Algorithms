#include "UndirectedGraphImage.h"
#include "Graph.h"
#include "VertexImage.h"
#include "Edge.h"
#include "EdgeImage.h"

UndirectedGraphImage::UndirectedGraphImage(GraphConfig * config, QGraphicsScene * scene)
: GraphImage(config, scene)
{
}


UndirectedGraphImage::~UndirectedGraphImage()
{
}

void UndirectedGraphImage::addEdge(int vertexId1, int vertexId2)
{
	int weight;
	if (_weighted)
	{
		bool succeeded = showEdgeImageDialog(vertexId1, vertexId2, weight);
		if (!succeeded)
			return;
	}
	Edge * edge = _graph->AddEdgeSingle(vertexId1, vertexId2);
	if (edge == nullptr)
		return;
	EdgeImage * edgeImg = createFullEdgeImage(edge, Application::Config::Instance().CurrentEdgeType());
	
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
	addEdgeImageToScene(edgeImg);
	updateVerticesDegree(edgeImg->VertexFrom(), edgeImg->VertexTo());
	return edgeImg;
}
