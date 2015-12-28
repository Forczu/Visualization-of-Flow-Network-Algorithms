void FlowNetworkAlgorithm::increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase)
{
	int oldFlow;
	EdgeImage * networkEdge;
	for (EdgeImage * edge : path)
	{
		int vertexFromId = edge->VertexFrom()->getId();
		int vertexToId = edge->VertexTo()->getId();
		networkEdge = network->edgeAt(vertexFromId, vertexToId);
		// jeżeli krawędź nie istnieje w prawdziwej sieci, należy utworzyć przepływ zwrotny
		if (networkEdge == nullptr)
		{
			networkEdge = network->edgeAt(vertexToId, vertexFromId);
			oldFlow = networkEdge->getFlow();
			networkEdge->setFlow(oldFlow - increase);
		}
		// jeżeli krawędź istnieje, ale posiada sąsiada, należy zmniejszyć jego przepływ
		else if (networkEdge != nullptr && networkEdge->hasNeighbor())
		{
			oldFlow = networkEdge->getFlow();
			EdgeImage * neighbourEdge = network->edgeAt(edge->VertexTo()->getId(), edge->VertexFrom()->getId());
			int currentNeighborFlow = neighbourEdge->getFlow();
			networkEdge->setFlow(std::max(increase - currentNeighborFlow, 0));
			neighbourEdge->setFlow(std::max(currentNeighborFlow - increase, 0));
		}
		// istnieje, ale nie posiada sąsiada, zwykłe zwiększenie
		else
		{
			oldFlow = networkEdge->getFlow();
			networkEdge->setFlow(oldFlow + increase);
		}
		edge->setSelected(false);
		networkEdge->setSelected(true);
	}
}