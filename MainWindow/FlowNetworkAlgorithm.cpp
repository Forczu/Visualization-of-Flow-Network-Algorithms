#include "FlowNetworkAlgorithm.h"

FlowNetworkAlgorithm::FlowNetworkAlgorithm() : _scaleFactor(0.0f), _currentMaxFlow(0)
{
}

void FlowNetworkAlgorithm::setCurrentMaxFlow(int flow)
{
	_currentMaxFlow = flow;
}
