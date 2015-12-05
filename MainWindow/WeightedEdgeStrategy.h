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

	bool addEdgeByDialog(int vertexId1, int vertexId2, int & capacity, int & flow) override
	{
		return showEdgeImageDialog(vertexId1, vertexId2, capacity, flow);
	}
	
	void addWeightToEdge(EdgeImage * edgeImg, QPointF const & pos, int capacity, float scale) override
	{
		edgeImg->setWeight(capacity);
		edgeImg->setTextItem(new EdgeTextItem(edgeImg, pos));
		edgeImg->scaleText(scale);
	}

	AWeightedStrategyBase * clone() const override
	{
		return getInstance();
	}

	bool showEdgeImageDialog(int vertexId1, int vertexId2, int & capacity, int & flow)
	{
		bool succeeded = false;
		AddWeightToEdgeDialog dialog(vertexId1, vertexId2);
		dialog.show();
		if (succeeded = (dialog.exec() == QDialog::Accepted))
		{
			flow = dialog.getFlow();
			capacity = dialog.getCapacity();
		}
		return succeeded;
	}

	void scaleText(EdgeImage * edgeImg, float scale) override
	{
		edgeImg->scaleText(scale);
	}

};