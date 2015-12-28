
bool FlowNetworkAlgorithm::checkExistingPathsToTarget(FlowNetwork * network)
{
	VertexImage * target = network->getTarget();
	bool pathExists = false;
	for (auto networkEdge : network->getEdges())
	{
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