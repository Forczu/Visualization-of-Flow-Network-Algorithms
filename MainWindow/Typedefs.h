#pragma once
#include <map>
#include <memory>

class Edge;
class EdgeImage;
class Vertex;
class VertexImage;
class TextItem;

//typedef	std::shared_ptr<Vertex>				VertexPtr;
//typedef std::weak_ptr<Vertex>				VertexWeakPtr;
//typedef std::shared_ptr<Edge>				EdgePtr;

typedef std::vector<Vertex*>				VertexVector;
typedef std::vector<Edge*>					EdgeVector;
typedef std::pair<VertexVector, EdgeVector>	GraphPair;

typedef std::map<int, VertexImage*>					VertexImageMap;
typedef std::map<std::pair<int, int>, EdgeImage*>	EdgeImageMap;
typedef std::map<std::string, TextItem*>			LabelMap;
