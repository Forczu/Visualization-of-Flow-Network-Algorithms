#include "GraphImage.h"
#include "Edge.h"
#include "EdgeImage.h"
#include "LoopEdgeImage.h"
#include "StraightEdgeImage.h"
#include "Vertex.h"
#include "VertexImage.h"
#include "Config.h"
#include "Graph.h"
#include "AddWeightToEdgeDialog.h"
#include "BezierEdgeImage.h"

GraphImage::GraphImage(GraphConfig * graphConfig) : _config(graphConfig)
{
	_graph = new Graph();
	setFlag(QGraphicsItem::ItemHasNoContents);
}

GraphImage::~GraphImage()
{
	for (VertexImageMap::iterator it = _vertexMap.begin(); it != _vertexMap.end(); ++it)
	{
		VertexImage * item = (*it).second;
		if (item) delete item;
	}
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		EdgeImage * item = (*it).second;
		if (item) delete item;
	}
}

void GraphImage::addVertex(QPointF const & position)
{
	Vertex * vertex = _graph->AddVertex();
	createVertexImage(vertex, position, vertex->Id());
}

void GraphImage::addVertex(int id, QPointF const & position)
{
	Vertex * vertex = _graph->AddVertex(id);
	createVertexImage(vertex, position, id);
}

void GraphImage::addVertex(int id, QPointF const & position, PointMap const & pointMap)
{
	Vertex * vertex = _graph->AddVertex(id);
	VertexImage * vertexImg = createVertexImage(vertex, position, id);
	vertexImg->setPoints(pointMap);
}

void GraphImage::addEdgeByDialog(int vertexId1, int vertexId2)
{
	int weight;
	if (_weighted)
	{
		bool succeeded = showEdgeImageDialog(vertexId1, vertexId2, weight);
		if (!succeeded)
			return;
	}
	addEdge(vertexId1, vertexId2, weight, Application::Config::Instance().CurrentEdgeType());
}

VertexImage * GraphImage::createVertexImage(Vertex * vertex, QPointF const & position, int id)
{
	VertexImage * vertexImg = new VertexImage(_config->NormalVertexContext()->clone());
	vertexImg->setVertex(vertex);
	vertexImg->setPos(position);
	vertexImg->setZValue(VERTICE_Z_VALUE);
	vertexImg->setParentItem(this);
	vertexImg->setParent(this);
	_vertexMap[id] = vertexImg;
	return vertexImg;
}

void GraphImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
}

QRectF GraphImage::boundingRect() const
{
	return childrenBoundingRect();
}

EdgeImage * GraphImage::createEdgeImage(Edge * edge, EdgeType edgeType)
{
	int size = _config->NormalVertexContext()->Size();
	EdgeImage * edgeImg;
	VertexImage * vertexFrom = _vertexMap[edge->VertexFrom()->Id()];
	VertexImage * vertexTo = _vertexMap[edge->VertexTo()->Id()];
	switch (edgeType)
	{
	case EdgeType::StraightLine: default:
		edgeImg = new StraightEdgeImage(edge, vertexFrom, vertexTo, _config->NormalEdgeContext()->clone());
		break;
	case EdgeType::BezierLine:
		edgeImg = new BezierEdgeImage(edge, vertexFrom, vertexTo, _config->NormalEdgeContext()->clone());
		break;
	}
	edgeImg->setFlag(QGraphicsItem::ItemIsMovable, false);
	edgeImg->setZValue(EDGE_Z_VALUE);
	edgeImg->setParentItem(this);
	_edgeMap[std::make_pair(edge->VertexFrom()->Id(), edge->VertexTo()->Id())] = edgeImg;
	return edgeImg;
}

bool GraphImage::showEdgeImageDialog(int vertexId1, int vertexId2, int & weight)
{
	bool succeeded = false;
	AddWeightToEdgeDialog dialog(vertexId1, vertexId2);
	dialog.show();
	dialog.exec();
	if (succeeded = dialog.isConfirmed())
		weight = dialog.getWeight();
	return succeeded;
}

void GraphImage::removeItem(QList<QGraphicsItem*> const & items)
{
	for each (QGraphicsItem* item in items)
	{
		try
		{
			VertexImage * vImg = dynamic_cast<VertexImage*>(item);
			if (NULL != vImg)
			{
				Vertex * vertex = vImg->getVertex();
				_graph->RemoveVertex(vertex);
				removeVertex(vImg);
				continue;
			}
			EdgeImage * eImg = dynamic_cast<EdgeImage*>(item);
			if (NULL != eImg)
			{
				Edge * edge = eImg->getEdge();
				_graph->RemoveEdge(edge);
				removeEdge(eImg);
				continue;
			}
		}
		catch (std::exception & e)
		{
		}
	}
}

void GraphImage::removeItem(QGraphicsItem * item)
{
	delete item;
}

void GraphImage::removeVertex(VertexImage * const vertex)
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end();)
	{
		EdgeImage * edge = (*it).second;
		if (edge->VertexFrom() == vertex || edge->VertexTo() == vertex)
		{
			removeItem(edge);
			it = _edgeMap.erase(it);
		}
		else
			++it;
	}
	removeItem(vertex);
}

void GraphImage::removeEdge(EdgeImage * const edge)
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		EdgeImage * item = (*it).second;
		if (edge == item)
		{
			removeItem(edge);
			_edgeMap.erase(it);
			break;
		}
	}
}

void GraphImage::removeEdges(EdgeVector const & vector)
{
	std::for_each(vector.begin(), vector.end(), [&](Edge * edge)
	{
		for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
		{
			if (edge == (*it).second->getEdge())
			{
				delete (*it).second;
				_edgeMap.erase(it);
				break;
			}
		}
	});
}

void GraphImage::correctNeighborEdges(Edge * const first, Edge * const second)
{
	EdgeImage * edgeImg;
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		edgeImg = it->second;
		if (edgeImg->VertexFrom()->getVertex() == first->VertexFrom())
		{
			edgeImg->correctEdge(true, EDGE_OFFSET);
		}
		else if (edgeImg->VertexTo()->getVertex() == second->VertexTo())
		{
			edgeImg->correctEdge(true, EDGE_OFFSET);
		}
	}
}

void GraphImage::changeEdge(EdgeImage * edgeImg, EdgeType type)
{
	StraightEdgeImage * seImg = dynamic_cast<StraightEdgeImage*>(edgeImg);
	if (NULL != seImg && type != EdgeType::StraightLine)
	{
		Edge * edge = seImg->getEdge();
		int weight = seImg->getEdge()->getCapacity();
		removeEdge(edgeImg);
		createFullEdgeImage(edge, type);
		return;
	}
	BezierEdgeImage * beImg = dynamic_cast<BezierEdgeImage*>(edgeImg);
	if (NULL != beImg && type != EdgeType::BezierLine)
	{
		Edge * edge = beImg->getEdge();
		int weight = seImg->getEdge()->getCapacity();
		removeEdge(edgeImg);
		createFullEdgeImage(edge, type, weight);
		return;
	}
}

void GraphImage::updateScale(float scale)
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		(*it).second->scaleText(scale);
	}
}
