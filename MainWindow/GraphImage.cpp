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
#include "AWeightedStrategyBase.h"
#include <utility>

GraphImage::GraphImage(GraphConfig * graphConfig) : _config(graphConfig)
{
	_graph = new Graph();
#if RELEASE
	setFlag(QGraphicsItem::ItemHasNoContents);
#endif
}

GraphImage::GraphImage(GraphImage const & graph)
{
	setFlag(QGraphicsItem::ItemHasNoContents);
	_config = graph._config->clone();
	_edgeStrategy = graph._edgeStrategy->clone();
	_graph = new Graph();
	cloneVertices(graph);
}

void GraphImage::cloneVertices(GraphImage const & graph)
{
	VertexImageMap map = graph.getVertices();
	VertexImage * vertexImg;
	for (VertexImageMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		vertexImg = (*it).second;
		Vertex * vertex = vertexImg->getVertex();
		int id = vertex->Id();
		QPointF position = vertexImg->pos();
		addVertex(id, position, vertexImg->getPoints());
	}
}

void GraphImage::cloneEdges(GraphImage & graph)
{
	EdgeImageMap map = graph.getEdges();
	EdgeImage * newEdge;
	for (Edge * edge : graph.getGraph()->getEdges())
	{
		addEdge(edge->VertexFrom()->Id(), edge->VertexTo()->Id(),
			edge->getCapacity(), EdgeType::StraightLine, edge->getFlow());
	}
}

GraphImage::~GraphImage()
{
	for (VertexImageMap::iterator it = _vertexMap.begin(); it != _vertexMap.end(); ++it)
	{
		deleteItem((*it).second);
	}
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		deleteItem((*it).second);
	}
	delete _config;
	delete _graph;
	delete _edgeStrategy;
}

void GraphImage::deleteItem(QGraphicsItem * const item)
{
	if (!item)
		return
	item->setParentItem(NULL);
	delete item;
}

void GraphImage::addVertex(QPointF const & position)
{
	Vertex * vertex = _graph->addVertex();
	createVertexImage(vertex, position, vertex->Id());
}

void GraphImage::addVertex(int id, QPointF const & position)
{
	Vertex * vertex = _graph->addVertex(id);
	createVertexImage(vertex, position, id);
}

void GraphImage::addVertex(int id, QPointF const & position, PointMap const & pointMap)
{
	Vertex * vertex = _graph->addVertex(id);
	VertexImage * vertexImg = createVertexImage(vertex, position, id);
	vertexImg->setPoints(pointMap);
}

void GraphImage::addEdgeByDialog(int vertexId1, int vertexId2, float scale)
{
	int capacity, flow;
	bool succeeded = _edgeStrategy->addEdgeByDialog(vertexId1, vertexId2, capacity, flow);
	if (!succeeded)
		return;
	addEdge(vertexId1, vertexId2, capacity, Application::Config::Instance().CurrentEdgeType(), flow, scale);
}

VertexImage * GraphImage::createVertexImage(Vertex * vertex, QPointF const & position, int id)
{
	VertexImage * vertexImg = new VertexImage(_config->NormalVertexContext());
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
#if DEBUG
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
	painter->drawRect(boundingRect());
#endif
}

QRectF GraphImage::boundingRect() const
{
	return childrenBoundingRect();
}

void GraphImage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
}

EdgeImage * GraphImage::createEdgeImage(Edge * edge, EdgeType edgeType, int weight /*= 0*/, float scale /*= 0.0f*/)
{
	int size = _config->NormalVertexContext()->Size();
	EdgeImage * edgeImg;
	VertexImage * vertexFrom = _vertexMap[edge->VertexFrom()->Id()];
	VertexImage * vertexTo = _vertexMap[edge->VertexTo()->Id()];
	switch (edgeType)
	{
	case EdgeType::StraightLine: default:
		edgeImg = new StraightEdgeImage(edge, vertexFrom, vertexTo, _config->NormalEdgeContext());
		break;
	case EdgeType::BezierLine:
		edgeImg = new BezierEdgeImage(edge, vertexFrom, vertexTo, _config->NormalEdgeContext());
		break;
	}
	edgeImg->setFlag(QGraphicsItem::ItemIsMovable, false);
	edgeImg->setZValue(EDGE_Z_VALUE);
	edgeImg->setParentItem(this);
	_edgeStrategy->addWeightToEdge(edgeImg, vertexTo->pos() - vertexFrom->pos(), weight, scale);
	_edgeMap[std::make_pair(edge->VertexFrom()->Id(), edge->VertexTo()->Id())] = edgeImg;
	return edgeImg;
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
				removeVertex(vImg);
				_graph->removeVertex(vertex);
				continue;
			}
			EdgeImage * eImg = dynamic_cast<EdgeImage*>(item);
			if (NULL != eImg)
			{
				Edge * edge = eImg->getEdge();
				removeEdge(eImg);
				_graph->removeEdge(edge);
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
	int id = vertex->getVertex()->Id();
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
	_vertexMap.erase(id);
	removeItem(vertex);
}

void GraphImage::removeEdge(EdgeImage * const edge)
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		EdgeImage * item = (*it).second;
		if (edge == item)
		{
			removeOffsetFromEdge(edge);
			getGraph()->removeEdge(edge->getEdge());
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
	const int MAX = 2;
	int count = 0;
	EdgeImage * edgeImg;
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		edgeImg = it->second;
		if (edgeImg->getEdge() == first || edgeImg->getEdge() == second)
		{
			edgeImg->correctEdge(true, EDGE_OFFSET);
			++count;
		}
		if (count == MAX)
			break;
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
		_edgeStrategy->scaleText((*it).second, scale);
	}
}

void GraphImage::unselectAll()
{
	for (auto vertex : _vertexMap)
	{
		if (vertex.second->isSelected())
			vertex.second->setSelected(false);
	}
	for (auto edge : _edgeMap)
	{
		if (edge.second->isSelected())
			edge.second->setSelected(false);
	}
}

void GraphImage::updateEdges()
{
	for (auto edge : _edgeMap)
	{
		edge.second->checkNewLine();
	}
}

void GraphImage::removeOffsetFromEdge(EdgeImage * const edge)
{
	int edgeId = edge->getEdge()->Id();
	edge->VertexFrom()->removePointForEdge(edgeId);
	edge->VertexTo()->removePointForEdge(edgeId);
	for (auto e : _edgeMap)
	{
		EdgeImage* img = e.second;
		if (img->VertexFrom() == edge->VertexTo() &&
			img->VertexTo() == edge->VertexFrom())
		{
			img->setOffset(false);
			img->checkNewLine();
			break;
		}
	}
}
