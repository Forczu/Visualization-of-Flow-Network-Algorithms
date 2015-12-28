/// <summary>
/// Przeszukanie przepływu blokującego w celu znalezienia ścieżki powiększającej.
/// Jeżeli isnieje, dodaje ją i przepływ do kontenerów.
/// </summary>
void BlockingFlowAlgorithmWindow::findAugumentingPathInBlockingFlow()
{
	_currentCapacity = 0;
	_currentBlockingPath = _algorithm->findAugumentingPath(_blockingFlow, _currentCapacity);
	if (_currentBlockingPath.size() != 0)
	{
		pushBlockingSet(_currentBlockingPath, _currentCapacity);
	}
}