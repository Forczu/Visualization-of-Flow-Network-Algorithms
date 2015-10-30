#include "DirectedGraphImage.h"
#include "Graph.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "Edge.h"

DirectedGraphImage::DirectedGraphImage(GraphConfig * config, QGraphicsScene * scene)
: GraphImage(config, scene)
{
}

DirectedGraphImage::~DirectedGraphImage()
{
}

void DirectedGraphImage::addEdge(int vertexId1, int vertexId2)
{
	int weight;
	if (_weighted)
	{
		bool succeeded = showEdgeImageDialog(vertexId1, vertexId2, weight);
		if (!succeeded)
			return;
	}
	Edge * edge = _graph->AddEdge(vertexId1, vertexId2);
	if (edge == nullptr)
		return;
	EdgeImage * img = createFullEdgeImage(edge, Application::Config::Instance().CurrentEdgeType(), weight);
}

void DirectedGraphImage::updateVerticesDegree(VertexImage * vertexFrom, VertexImage * vertexTo)
{
	std::pair<int, int> degree = _graph->getDegree(vertexFrom->getVertex());
	vertexFrom->setToolTip(degree.first, degree.second);
	degree = _graph->getDegree(vertexTo->getVertex());
	vertexTo->setToolTip(degree.first, degree.second);
}

EdgeImage * DirectedGraphImage::createFullEdgeImage(Edge * edge, EdgeType type, int weight /*= 0*/)
{
	EdgeImage * edgeImg = createEdgeImage(edge, type);
	if (edgeImg == nullptr)
		return edgeImg;
	edgeImg->setWeight(weight);
	addEdgeImageToScene(edgeImg);
	edgeImg->addArrowHead();
	Edge * neighbor = _graph->GetNeighborEdge(edge);
	if (neighbor != nullptr)
		correctNeighborEdges(edge, neighbor);
	updateVerticesDegree(edgeImg->VertexFrom(), edgeImg->VertexTo());
	return edgeImg;
}
