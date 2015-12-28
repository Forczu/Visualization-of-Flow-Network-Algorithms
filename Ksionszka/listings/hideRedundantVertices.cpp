/// <summary>
/// Ukrywa nadmiarowe wierzchołki, które nie pojawią się w przepływie blokującym.
/// Przeszukiwanie przepływu odbywa się wyłącznie poprzez krawędzie, więc wierzchołki
/// wystarczy ukryć, nie trzeba ich usuwać.
/// Schowane zostają wierzchołki, które spełniają warunek $ v\in V\backslash\{t\} : d_{min}(s,t)\le d_{min}(s,v) $
/// </summary>
/// <param name="residualNewtork">Sieć przepływowa.</param>
void BlockingFlowAlgoritm::hideRedundantVertices(FlowNetwork *& residualNewtork)
{
	for (auto vertex : residualNewtork->getVertices())
	{
		int vertexId = vertex->getId() - 1;
		if (_pathMatrix[_sourceId][_targetId] <= _pathMatrix[_sourceId][vertexId] && vertexId != _targetId)
		{
			_currentHiddenVertices.push_back(vertex);
			vertex->hide();
		}
	}
}