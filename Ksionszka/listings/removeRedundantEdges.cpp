/// <summary>
/// Usuwa nadmiarowe krawędzie, które nie biorą udziału w tworzeniu przepływu blokującego w warstwowej sieci residualnej.
/// Usuwane są krawędzie, które spelniają warunek $ (v_j,v_i)\in E : d_{min}(s,v_i)\le d_{min}(s,v_j) $
/// </summary>
/// <param name="residualNewtork">Sieć przepływowa.</param>
void BlockingFlowAlgoritm::removeRedundantEdges(FlowNetwork *& residualNewtork)
{
	QList<EdgeImage*> edgesToRemove;
	for (auto edge : residualNewtork->getEdges())
	{
		int from = edge->VertexFrom()->getId() - 1;
		int to = edge->VertexTo()->getId() - 1;
		int first = _pathMatrix[_sourceId][to];
		int second = _pathMatrix[_sourceId][from];
		if (first <= second)
			edgesToRemove.push_back(edge);
	}
	for (auto edge : edgesToRemove)
	{
		residualNewtork->removeEdge(edge);
	}
	for (auto vertex : _currentHiddenVertices)
	{
		for (auto edge : residualNewtork->getEdges())
		{
			if (edge->VertexFrom() == vertex || edge->VertexTo() == vertex)
			{
				residualNewtork->removeEdge(edge);
			}
		}
	}
}