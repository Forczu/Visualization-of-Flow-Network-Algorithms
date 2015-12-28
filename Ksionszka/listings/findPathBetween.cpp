QList<EdgeImage*> FlowNetworkAlgorithm::findPathBetween(FlowNetwork * network, VertexImage * from, VertexImage * to)
{
	QList<EdgeImage*> path;
	VertexImage * source = network->getSource();
	EdgeImageMap edges = network->getEdges();
	bool finished = false;
	VertexImage * currentVertex = from;
	QList<VertexImage*> visitedVertices;
	QList<VertexImage*> rejectedVertices;
	EdgeImage * lastEdge = nullptr;
	srand(time(NULL));
	while (!finished)
	{
		QList<EdgeImage*> possibleEdges;
		for (auto edge : edges)
		{
			addEdgeToPath(possibleEdges, edge, currentVertex, source, visitedVertices, rejectedVertices);
		}
		if (possibleEdges.empty())
		{
			if (lastEdge == nullptr)
			{
				finished = true;
				return path;
			}
			else
			{
				rejectedVertices.push_back(currentVertex);
				path.pop_back();
				if (!path.empty())
				{
					lastEdge = path.last();
					currentVertex = lastEdge->VertexTo();
				}
				else
				{
					lastEdge = nullptr;
					currentVertex = from;
				}
				continue;
			}
		}
		EdgeImage * chosenEdge = possibleEdges.at(rand() % possibleEdges.size());
		VertexImage * nextVertex = chosenEdge->VertexTo();
		visitedVertices.push_back(nextVertex);
		path.push_back(chosenEdge);
		currentVertex = nextVertex;
		lastEdge = chosenEdge;
		if (currentVertex == to)
			finished = true;
	}
	return path;
}

void FordFulkersonAlgorithm::addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source, QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices)
{
	if (edge->VertexFrom() == currentVertex &&
		edge->VertexTo() != source &&
		!visitedVertices.contains(edge->VertexTo()) &&
		!rejectedVertices.contains(edge->VertexTo()))
	{
		possibleEdges.push_back(edge);
	}
}