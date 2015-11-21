#pragma once
#include "AWeightedStrategyBase.h"
#include "EdgeImage.h"
#include "AddWeightToEdgeDialog.h"

class WeightedEdgeStrategy : public AWeightedStrategyBase
{
public:
	static AWeightedStrategyBase * getInstance()
	{
		return new WeightedEdgeStrategy;
	}

	bool addEdgeByDialog(int vertexId1, int vertexId2, int & weight) override
	{
		return showEdgeImageDialog(vertexId1, vertexId2, weight);
	}
	
	void addWeightToEdge(EdgeImage * edgeImg, int weight) override
	{
		edgeImg->setWeight(weight);
		edgeImg->setTextItem(new EdgeTextItem(edgeImg, QPointF()));
	}

	AWeightedStrategyBase * clone() const override
	{
		return getInstance();
	}

	bool showEdgeImageDialog(int vertexId1, int vertexId2, int & weight)
	{
		bool succeeded = false;
		AddWeightToEdgeDialog dialog(vertexId1, vertexId2);
		dialog.show();
		dialog.exec();
		if (succeeded = dialog.isConfirmed())
			weight = dialog.getWeight();
		return succeeded;
	}
};