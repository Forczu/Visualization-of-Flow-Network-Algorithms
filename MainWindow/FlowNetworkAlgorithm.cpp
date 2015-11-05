#include "FlowNetworkAlgorithm.h"

void FlowNetworkAlgorithm::setNetwork(FlowNetwork * network)
{
	_network = network;
}

FlowNetworkAlgorithm::FlowNetworkAlgorithm() : _network(nullptr)
{
}
