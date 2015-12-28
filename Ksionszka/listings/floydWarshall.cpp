/// <summary>
/// Tworzy macierz najkrótszych dróg z każdego wierzchołka do innych.
/// Długości dróg obliczane są algorytmem Floyda-Warshalla.
/// Ponieważ krawędzi w sieci przepływowej posiadają przepustowość i przepływ,
/// zakłada się, że każda droga między sąsiednimi wierzchołkami jest równa 1.
/// </summary>
/// <param name="newtork">Siec przepływowa.</param>
void BlockingFlowAlgoritm::createShortestPathsMatrix(FlowNetwork *& newtork)
{
	const int n = newtork->getHighestVertexId();
	_pathMatrix = FloatMatrix(n);
	for (int i = 0; i < n; ++i)
	{
		_pathMatrix[i].resize(n);
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (i != j)
				_pathMatrix[i][j] = std::numeric_limits<float>::infinity();
		}
	}
	for (auto coord : newtork->getEdges().keys())
	{
		_pathMatrix[coord.first - 1][coord.second - 1] = 1.0f;
	}
	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
		{
			if (_pathMatrix[i][k] != std::numeric_limits<float>::infinity())
			{
				for (int j = 0; j < n; ++j)
				{
					float first = _pathMatrix[i][j];
					float second = _pathMatrix[i][k] + _pathMatrix[k][j];
					_pathMatrix[i][j] = std::min(first, second);
				}
			}
		}
	}
}