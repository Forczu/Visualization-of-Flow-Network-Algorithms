int FlowNetworkAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork)
{
	// usunięcie starych krawędzi
	auto oldEdges = outResidaulNetwork->getEdges();
	for (auto it = oldEdges.begin(); it != oldEdges.end(); ++it)
	{
		outResidaulNetwork->removeEdge(*it);
	}
	// analiza krawędzi i utworzenie sieci residualnej
	auto edges = network->getEdges();
	QList<EdgeImage*> visitedNeighbours;
	EdgeImage *neighbor;
	for (EdgeImage * edge : edges)
	{
		int capacity = edge->getCapacity();
		int flow = edge->getFlow();
		int vertexFromId = edge->VertexFrom()->getId();
		int vertexToId = edge->VertexTo()->getId();
		int residualCapacity = capacity - flow;
		if (edge->hasNeighbor() && !visitedNeighbours.contains(neighbor = network->edgeAt(vertexToId, vertexFromId)))
		{
			visitedNeighbours.push_back(neighbor);;
			visitedNeighbours.push_back(edge);
			int neighborFlow = neighbor->getFlow();
			int neighborCapacity = neighbor->getCapacity();
			residualCapacity = capacity - flow + neighborFlow;
			int neighborResidualCapacity = neighborCapacity - neighborFlow + flow;
			if (residualCapacity != 0)
				outResidaulNetwork->addEdge(vertexFromId, vertexToId, residualCapacity);
			if (neighborResidualCapacity != 0)
				outResidaulNetwork->addEdge(vertexToId, vertexFromId, neighborResidualCapacity);
		}
		else
		{
			if (flow != 0)
				outResidaulNetwork->addEdge(vertexToId, vertexFromId, flow);
			if (residualCapacity != 0)
				outResidaulNetwork->addEdge(vertexFromId, vertexToId, residualCapacity);
		}
	}
	return 0;
}