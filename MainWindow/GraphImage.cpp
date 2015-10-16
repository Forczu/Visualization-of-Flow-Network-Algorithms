#include "GraphImage.h"
#include "Edge.h"
#include "EdgeImage.h"
#include "LoopEdgeImage.h"
#include "StraightEdgeImage.h"
#include "Vertex.h"
#include "VertexImage.h"
#include "Config.h"
#include "Graph.h"

GraphImage::GraphImage(QGraphicsScene * scene) : _scene(scene)
{
	_graph = new Graph();
}

GraphImage::~GraphImage()
{
	for (VertexImageMap::iterator it = _vertexMap.begin(); it != _vertexMap.end(); ++it)
	{
		delete (*it).second;
	}
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		delete (*it).second;
	}
}

void GraphImage::addVertex(QPointF const & position)
{
	Vertex * vertex = _graph->AddVertex();
	VertexImage * vertexImg = new VertexImage(Application::Config::Instance().DefaultVertexContext());
	vertexImg->setVertex(vertex);
	vertexImg->setPos(position);
	vertexImg->setFlag(QGraphicsItem::ItemIsMovable, true);
	vertexImg->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	vertexImg->setZValue(VERTICE_Z_VALUE);
	_scene->addItem(vertexImg);
	_vertexMap[vertex->Id()] = vertexImg;
}

void GraphImage::addEdge(int vertexId1, int vertexId2, QPointF const & p1, QPointF const & p2)
{
	Edge * edge = _graph->AddEdge(vertexId1, vertexId2);
	if (edge == nullptr)
		return;

	int size = Application::Config::Instance().DefaultVertexContext().Size();
	EdgeImage * edgeImg;
	VertexImage * vertexFrom = _vertexMap[edge->VertexFrom()->Id()];
	VertexImage * vertexTo = _vertexMap[edge->VertexTo()->Id()];
	if (std::abs(p1.x() - p2.x()) <= size && std::abs(p1.y() - p2.y()) <= size)
	{
		edgeImg = new LoopEdgeImage(edge, vertexFrom, vertexTo, Application::Config::Instance().DefaultEdgeContext());
	}
	else
	{
		edgeImg = new StraightEdgeImage(edge, vertexFrom, vertexTo, Application::Config::Instance().DefaultEdgeContext());
	}
	edgeImg->setFlag(QGraphicsItem::ItemIsMovable, false);
	edgeImg->setZValue(EDGE_Z_VALUE);
	_scene->addItem(edgeImg);
	_edgeMap[std::make_pair(edge->VertexFrom()->Id(), edge->VertexTo()->Id())] = edgeImg;
	if (Application::Config::Instance().IsGraphDirected())
		edgeImg->addArrowHead();

	Edge * neighbor = _graph->GetNeighborEdge(edge);
	if (neighbor != nullptr)
		correctNeighborEdges(edge, neighbor);
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
	item->setParentItem(NULL);
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

void GraphImage::makeDirected()
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		(*it).second->addArrowHead();
	}
}

void GraphImage::makeUndirected()
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		(*it).second->deleteArrowHead();
	}
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
