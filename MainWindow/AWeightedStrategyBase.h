#pragma once
#include "ICloneable.h"
#include "EdgeImage.h"
class EdgeImage;

class AWeightedStrategyBase : public ICloneable<AWeightedStrategyBase>
{
public:
	virtual bool addEdgeByDialog(int vertexId1, int vertexId2, int & capacity, int & flow) = 0;
	virtual void addWeightToEdge(EdgeImage * edgeImg, QPointF const & pos, int weight, float scale = 0.0f) = 0;
	virtual void scaleText(EdgeImage * edgeImg, float scale) = 0;
};