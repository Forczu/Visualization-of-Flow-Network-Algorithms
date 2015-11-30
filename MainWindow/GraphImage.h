#pragma once
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QObject>
#include <QAction>
#include <vector>
#include "Typedefs.h"
#include "GraphConfig.h"
#include "Edges.h"
#include "Consts.h"
#include "Graph.h"
#include "ICloneable.h"

class AWeightedStrategyBase;
class Edge;
class EdgeImage;
class Vertex;
class VertexImage;

class GraphImage : public QObject, public QGraphicsItem, public ICloneable<GraphImage>
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
protected:
	GraphConfig * _config;
	Graph * _graph;
	VertexImageMap	_vertexMap;
	EdgeImageMap	_edgeMap;
	AWeightedStrategyBase * _edgeStrategy;

public:
	GraphImage(GraphConfig * graphConfig);
	GraphImage(GraphImage const & graph);
	void cloneVertices(GraphImage const & graph);
	void cloneEdges(GraphImage & graph);
	GraphImage * createCopy();
	virtual ~GraphImage();
private:
	void deleteItem(QGraphicsItem * const item);
	void init();
public:
	void addVertex(QPointF const & position);
	void addVertex(int id, QPointF const & position);
	void addVertex(int id, QPointF const & position, PointMap const & pointMap);
	inline VertexImage * vertexAt(int id) { return _vertexMap[id]; }

	void addEdgeByDialog(int vertexId1, int vertexId2, float scale);
	virtual EdgeImage * addEdge(int vertexId1, int vertexId2, int weight, EdgeType type, int flow = 0, float scale = 1.0f) = 0;
	virtual EdgeImage * createFullEdgeImage(Edge * edge, EdgeType type, int weight = 0, int flow = 0, float scale = 1.0f) = 0;

protected:
	EdgeImage * createEdgeImage(Edge * edge, EdgeType edgeType, int weight = 0, float scale = 1.0f);
	VertexImage * createVertexImage(Vertex * vertex, QPointF const & position, int id);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;
	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

public:
	void removeItem(QList<QGraphicsItem*> const & items);
	void removeItem(QGraphicsItem * item);
	void removeVertex(VertexImage * const vertex);
	void removeEdge(EdgeImage * const edge);
	void removeEdges(EdgeVector const & vector);
	void correctNeighborEdges(Edge * const first, Edge * const second);

	inline Graph * getGraph() const { return _graph; }
	void setGraph(Graph * val) { _graph = val; }
	inline AWeightedStrategyBase * getWeightStrategy() const { return _edgeStrategy; }
	void setWeightStrategy(AWeightedStrategyBase * val) { _edgeStrategy = val; }
	inline GraphConfig * getConfig() const { return _config; }
	void setConfig(GraphConfig * val) { _config = val; }
	inline VertexImageMap getVertices() const { return _vertexMap; }
	inline EdgeImageMap getEdges() const { return _edgeMap; }
	void setName(QString const & name) { _config->setName(name); }
	inline QString getName() const { return _config->getName(); }

	EdgeImage * edgeAt(int from, int to);
	
	void changeEdge(EdgeImage * edge, EdgeType type);
	void updateScale(float scale);
	void unselectAll();
	void updateEdges();
	void removeOffsetFromEdge(EdgeImage * const edge);
	inline bool edgeExists(int from, int to) { return getGraph()->edgeExists(from, to); }
};

