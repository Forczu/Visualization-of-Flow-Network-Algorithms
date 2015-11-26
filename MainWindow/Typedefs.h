#pragma once
#include <map>
#include <QMap>
#include <memory>
#include <QPointF>

class AWeightedStrategyBase;
class Edge;
class EdgeImage;
class GraphImage;
class GraphConfig;
class Vertex;
class VertexImage;
class TextItem;

static const float ANGLE_INTERVAL = 5.0f;

typedef std::vector<Vertex*>				VertexVector;
typedef std::vector<Edge*>					EdgeVector;
typedef std::pair<VertexVector, EdgeVector>	GraphPair;

typedef std::map<int, VertexImage*>					VertexImageMap;
typedef std::map<std::pair<int, int>, EdgeImage*>	EdgeImageMap;
typedef std::map<std::string, TextItem*>			LabelMap;

typedef std::pair<int, QPointF>		PointPair;
typedef std::map<int, QPointF>		PointMap;

typedef GraphImage* (*GraphCreateFunc)(GraphConfig * config);
typedef AWeightedStrategyBase* (*EdgeStrategyCreateFunc)();