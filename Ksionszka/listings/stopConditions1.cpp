bool FlowNetworkAlgorithm::checkExistingPathsFromSource(FlowNetwork * network)
{
	VertexImage * source = network->getSource();
	bool pathExists = false;
	for (auto networkEdge : network->getEdges())
	{
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