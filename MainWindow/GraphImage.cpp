#include "GraphImage.h"
#include "Edge.h"
#include "EdgeImage.h"
#include "StraightEdgeImage.h"
#include "Vertex.h"
#include "VertexImage.h"
#include "Config.h"
#include "Graph.h"
#include "BezierEdgeImage.h"
#include "AWeightedStrategyBase.h"
#include <utility>

GraphImage::GraphImage(GraphConfig * graphConfig) : _config(graphConfig), _edgeStrategy(nullptr)
{
	init();
}

GraphImage::GraphImage(GraphImage const & graph)
{
	init();
	_config = graph._config->clone();
	_edgeStrategy = graph._edgeStrategy->clone();
	cloneVertices(graph);
}

void GraphImage::init()
{
	_graph = new Graph();
#if RELEASE
	setFlag(QGraphicsItem::ItemHasNoContents);
#endif
}

void GraphImage::cloneVertices(GraphImage const & graph)
{
	VertexImageMap map = graph.getVertices();
	for (VertexImageMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		int id = (*it)->getId();
		QPointF position = (*it)->pos();
		addVertex(id, position, (*it)->getPoints());
		if (!(*it)->isVisible())
		{
			VertexImage * newVertex = vertexAt(id);
			newVertex->hide();
		}
	}
}

void GraphImage::cloneEdges(GraphImage & graph)
{
	EdgeImageMap map = graph.getEdges();
	for (Edge * edge : graph.getGraph()->getEdges())
	{
		addEdge(edge->VertexFrom()->Id(), edge->VertexTo()->Id(),
			edge->getCapacity(), EdgeType::StraightLine, edge->getFlow());
	}
}

GraphImage * GraphImage::createCopy()
{
	auto copy = clone();
	copy->setPos(pos());
	return copy;
}

GraphImage::~GraphImage()
{
	for (VertexImageMap::iterator it = _vertexMap.begin(); it != _vertexMap.end(); ++it)
	{
		deleteItem(*it);
	}
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		deleteItem(*it);
	}
	delete _config;
	delete _graph;
	delete _edgeStrategy;
}

void GraphImage::deleteItem(QGraphicsItem * const item)
{
	if (!item)
		return;
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
	edgeImg->setFlag(QGraphicsItem::ItemIsSelectable, true);
	edgeImg->setZValue(EDGE_Z_VALUE);
	edgeImg->setParentItem(this);
	edgeImg->setParent(this);
	_edgeStrategy->addWeightToEdge(edgeImg, vertexTo->pos() - vertexFrom->pos(), weight, scale);
	_edgeMap[std::make_pair(edge->VertexFrom()->Id(), edge->VertexTo()->Id())] = edgeImg;
	vertexFrom->setOutDegree(vertexFrom->getOutDegree() + 1);
	vertexTo->setInDegree(vertexTo->getInDegree() + 1);
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
	delete item;
}

void GraphImage::removeVertex(VertexImage * const vertex)
{
	int id = vertex->getId();
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end();)
	{
		if ((*it)->VertexFrom() == vertex || (*it)->VertexTo() == vertex)
		{
			removeItem(*it);
			it = _edgeMap.erase(it);
		}
		else
			++it;
	}
	_vertexMap.remove(id);
	removeItem(vertex);
}

void GraphImage::removeEdge(EdgeImage * const edge)
{
	auto key = std::make_pair(edge->VertexFrom()->getId(), edge->VertexTo()->getId());
	if (_edgeMap.contains(key))
	{
		VertexImage * from = edge->VertexFrom();
		VertexImage * to = edge->VertexTo();
		from->setOutDegree(from->getOutDegree() - 1);
		to->setInDegree(to->getInDegree() - 1);
		removeOffsetFromEdge(edge);
		getGraph()->removeEdge(edge->getEdge());
		removeItem(edge);
		_edgeMap.remove(key);
	}
}

void GraphImage::removeEdges(EdgeVector const & vector)
{
	std::for_each(vector.begin(), vector.end(), [&](Edge * edge)
	{
		for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
		{
			if (edge == (*it)->getEdge())
			{
				VertexImage * from = (*it)->VertexFrom();
				VertexImage * to = (*it)->VertexTo();
				from->setOutDegree(from->getOutDegree() - 1);
				to->setInDegree(to->getInDegree() - 1);
				delete *it;
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
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		if ((*it)->getEdge() == first || (*it)->getEdge() == second)
		{
			(*it)->correctEdge(true, EDGE_OFFSET);
			(*it)->setNeighbor(true);
			++count;
		}
		if (count == MAX)
			break;
	}
}

void GraphImage::setConfig(GraphConfig * val)
{
	if (_config == val)
		return;
	delete _config;
	_config = val;
	for (auto vertex : _vertexMap)
	{
		if (!vertex->isSelected())
			vertex->setContext(_config->NormalVertexContext());
		else
			vertex->setContext(_config->SelectedVertexContext());
	}
	for (auto edge : _edgeMap)
	{
		if (!edge->isSelected())
			edge->setContext(_config->NormalEdgeContext());
		else
			edge->setContext(_config->SelectedEdgeContext());
	}
}

/// <summary>
/// Zwraca wskaŸnik na krawêdŸ na podstawie pary kluczy.
/// </summary>
/// <param name="from">Pocz¹tek krawêdzi.</param>
/// <param name="to">Koniec krawêdzi.</param>
/// <returns>KrawêdŸ, je¿eli istnieje, nullptr, je¿eli nie istnieje</returns>
EdgeImage * GraphImage::edgeAt(int from, int to)
{
	std::pair<int, int> key(from, to);
	if (_edgeMap.find(key) != _edgeMap.end())
		return _edgeMap[std::pair<int, int>(from, to)];
	else
		return nullptr;
}

void GraphImage::changeEdge(EdgeImage * edgeImg, EdgeType type)
{
	StraightEdgeImage * seImg = dynamic_cast<StraightEdgeImage*>(edgeImg);
	if (NULL != seImg && type != EdgeType::StraightLine)
	{
		Edge * edge = seImg->getEdge();
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
	}
}

void GraphImage::updateScale(float scale)
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		_edgeStrategy->scaleText(*it, scale);
	}
}

void GraphImage::unselectAll()
{
	for (auto vertex : _vertexMap)
	{
		if (vertex->isSelected())
			vertex->setSelected(false);
	}
	for (auto edge : _edgeMap)
	{
		if (edge->isSelected())
			edge->setSelected(false);
	}
}

void GraphImage::updateEdges()
{
	for (auto edge : _edgeMap)
	{
		edge->checkNewLine();
	}
}

void GraphImage::removeOffsetFromEdge(EdgeImage * const edge)
{
	int edgeId = edge->getEdge()->Id();
	edge->VertexFrom()->removePointForEdge(edgeId);
	edge->VertexTo()->removePointForEdge(edgeId);
	if (!edge->hasNeighbor())
		return;
	edge->setNeighbor(false);
	for (auto e : _edgeMap)
	{
		if (e->VertexFrom() == edge->VertexTo() &&
			e->VertexTo() == edge->VertexFrom())
		{
			e->setOffset(false);
			e->checkNewLine();
			e->setNeighbor(false);
			break;
		}
	}
}
