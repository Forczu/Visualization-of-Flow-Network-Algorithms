/// <summary>
/// Dodanie nowej ścieżki powiększającej do zbioru ścieżek.
/// </summary>
/// <param name="path">Ścieżka.</param>
/// <param name="capacity">Maksymalna wartość przeputowości residualnej.</param>
void BlockingFlowAlgorithmWindow::pushBlockingSet(QList<EdgeImage*> const & path, int capacity)
{
	QList<EdgeImage*> residualPath;
	for (auto edge : path)
	{
		int from = edge->VertexFrom()->getId();
		int to = edge->VertexTo()->getId();
		residualPath.push_back(_residualNetwork->edgeAt(from, to));
	}
	_paths.push_back(residualPath);
	_capacities.push_back(capacity);
}