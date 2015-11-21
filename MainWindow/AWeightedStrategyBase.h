#pragma once
#include "ICloneable.h"
class EdgeImage;

class AWeightedStrategyBase : public ICloneable<AWeightedStrategyBase>
{
public:
	virtual bool addEdgeByDialog(int vertexId1, int vertexId2, int & weight) = 0;
	virtual void addWeightToEdge(EdgeImage * edgeImg, int weight) = 0;
};