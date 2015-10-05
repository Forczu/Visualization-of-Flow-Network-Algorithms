#pragma once

#include <memory>

#include "Edge.h"
#include "Vertex.h"

//typedef	std::shared_ptr<Vertex>				VertexPtr;
//typedef std::weak_ptr<Vertex>				VertexWeakPtr;
//typedef std::shared_ptr<Edge>				EdgePtr;

typedef std::vector<Vertex*>				VertexVector;
typedef std::vector<Edge*>					EdgeVector;
typedef std::pair<VertexVector, EdgeVector>	GraphPair;
