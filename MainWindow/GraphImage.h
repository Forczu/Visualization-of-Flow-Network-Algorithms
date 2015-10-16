#pragma once
#include <QGraphicsScene>
#include <vector>
#include "Typedefs.h"

class Edge;
class EdgeImage;
class Graph;
class Vertex;
class VertexImage;

class GraphImage
{
protected:
	static const int ARROWHEAD_Z_VALUE = 3;
	static const int VERTICE_Z_VALUE = 2;
	static const int EDGE_Z_VALUE = 1;
	static const int EDGE_OFFSET = 30;

	QGraphicsScene * _scene;
	Graph * _graph;
	VertexImageMap	_vertexMap;
	EdgeImageMap	_edgeMap;

public:
	GraphImage(QGraphicsScene * scene);
	virtual ~GraphImage();

	void addVertex(QPointF const & position);
	virtual void addEdge(int vertexId1, int vertexId2, QPointF const & coord1, QPointF const & coord2) = 0;

protected:
	EdgeImage * CreateEdgeImage(Edge * edge, QPointF const &p1, QPointF const &p2);
public:
	void removeItem(QList<QGraphicsItem*> const & items);

	void removeItem(QGraphicsItem * item);
	void removeVertex(VertexImage * const vertex);
	void removeEdge(EdgeImage * const edge);
	void removeEdges(EdgeVector const & vector);

	void makeDirected();
	void makeUndirected();
	void correctNeighborEdges(Edge * const first, Edge * const second);

	inline Graph * getGraph() const { return _graph; }
	void setGraph(Graph * val) { _graph = val; }
};

