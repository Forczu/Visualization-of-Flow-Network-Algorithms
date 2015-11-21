#pragma once
#include "AWeightedStrategyBase.h"

class UnweightedEdgeStrategy : public AWeightedStrategyBase
{
public:
	static AWeightedStrategyBase * getInstance()
	{
		return new UnweightedEdgeStrategy;
	}

	bool addEdgeByDialog(int vertexId1, int vertexId2, int & weight) override
	{
		return true;
	}

	void addWeightToEdge(EdgeImage * edgeImg, int weight) override
	{
		return;
	}

	AWeightedStrategyBase * clone() const override
	{
		return getInstance();
	}
};