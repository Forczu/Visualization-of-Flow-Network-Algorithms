#pragma once
#include "AWeightedStrategyBase.h"

class UnweightedEdgeStrategy : public AWeightedStrategyBase
{
public:
	static AWeightedStrategyBase * getInstance()
	{
		return new UnweightedEdgeStrategy;
	}

	bool addEdgeByDialog(int vertexId1, int vertexId2, int & capacity, int & flow) override
	{
		return true;
	}

	void addWeightToEdge(EdgeImage * edgeImg, QPointF const & pos, int weight, float scale) override
	{
		return;
	}

	AWeightedStrategyBase * clone() const override
	{
		return getInstance();
	}

	void scaleText(EdgeImage * edgeImg, float scale) override
	{
		return;
	}

};