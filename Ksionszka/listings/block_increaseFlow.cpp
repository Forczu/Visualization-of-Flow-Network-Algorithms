/// <summary>
/// Zwiększa przepływ w sieci przepływającej o dodatkową wartość w znalezionej ścieżce.
/// </summary>
void FlowNetworkAlgorithmWindow::increaseFlow()
{
	for (int i = 0; i < _capacities.size(); ++i)
	{
		_algorithm->increaseFlow(_network, _paths[i], _capacities[i]);
	}
	clearSets();
}