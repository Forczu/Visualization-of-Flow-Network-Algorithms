#include "FlowNetworkAlgorithm.h"
#include "EdgeImage.h"

FlowNetworkAlgorithm::FlowNetworkAlgorithm() : _scaleFactor(0.0f), _currentMaxFlow(0)
{
}

void FlowNetworkAlgorithm::setCurrentMaxFlow(int flow)
{
	_currentMaxFlow = flow;
}

bool FlowNetworkAlgorithm::checkExistingPathsFromSource(FlowNetwork * network)
{
	EdgeImage * networkEdge;
	VertexImage * source = network->getSource();
	bool pathExists = false;
	for (auto item : network->getEdges())
	{
		networkEdge = item.second;
		if (networkEdge->VertexFrom() == source)
			if (networkEdge->getFlow() == networkEdge->getCapacity())
				continue;
			else
			{
				pathExists = true;
				break;
			}
	}
	return pathExists;
}

bool FlowNetworkAlgorithm::checkExistingPathsToTarget(FlowNetwork * network)
{
	EdgeImage * networkEdge;
	VertexImage * target = network->getTarget();
	bool pathExists = false;
	for (auto item : network->getEdges())
	{
		networkEdge = item.second;
		if (networkEdge->VertexTo() == target)
		{
			if (networkEdge->getFlow() == networkEdge->getCapacity())
				continue;
			else
			{
				pathExists = true;
				break;
			}
		}
	}
	return pathExists;
}
