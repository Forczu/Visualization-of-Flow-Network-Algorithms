#pragma once
#include "ICloneable.h"
#include "EdgeImage.h"
class EdgeImage;

class AWeightedStrategyBase : public ICloneable<AWeightedStrategyBase>
{
public:
	virtual bool addEdgeByDialog(int vertexId1, int vertexId2, int & weight) = 0;
	virtual void addWeightToEdge(EdgeImage * edgeImg, int weight) = 0;
	virtual void scaleText(EdgeImage * edgeImg, float scale) = 0;
};