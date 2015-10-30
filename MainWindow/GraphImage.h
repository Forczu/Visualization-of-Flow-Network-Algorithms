#pragma once
#include <QGraphicsScene>
#include <QObject>
#include <QAction>
#include <vector>
#include "Typedefs.h"
#include "GraphConfig.h"
#include "Edges.h"
#include "Consts.h"

class Edge;
class EdgeImage;
class Graph;
class Vertex;
class VertexImage;

class GraphImage : public QObject
{
	Q_OBJECT

protected:
	GraphConfig * _config;
	QGraphicsScene * _scene;
	Graph * _graph;
	VertexImageMap	_vertexMap;
	EdgeImageMap	_edgeMap;

	bool _weighted;
public:
	GraphImage(GraphConfig * graphConfig, QGraphicsScene * scene);
	virtual ~GraphImage();

	void addVertex(QPointF const & position);
	virtual void addEdge(int vertexId1, int vertexId2) = 0;
	virtual EdgeImage * createFullEdgeImage(Edge * edge, EdgeType type, int weight = 0) = 0;

protected:
	EdgeImage * createEdgeImage(Edge * edge, EdgeType edgeType);
	bool showEdgeImageDialog(int vertexId1, int vertexId2, int & weight);
	void addEdgeImageToScene(EdgeImage * edgeImage);

public:
	void removeItem(QList<QGraphicsItem*> const & items);
	void removeItem(QGraphicsItem * item);
	void removeVertex(VertexImage * const vertex);
	void removeEdge(EdgeImage * const edge);
	void removeEdges(EdgeVector const & vector);

	void correctNeighborEdges(Edge * const first, Edge * const second);

	inline Graph * getGraph() const { return _graph; }
	void setGraph(Graph * val) { _graph = val; }
	inline bool Weighted() const { return _weighted; }
	void Weighted(bool val) { _weighted = val; }
	inline GraphConfig * getConfig() const { return _config; }
	void setConfig(GraphConfig * val) { _config = val; }
	inline VertexImageMap getVertices() const { return _vertexMap; }
	inline EdgeImageMap getEdges() const { return _edgeMap; }
	
	void changeEdge(EdgeImage * edge, EdgeType type);
};

