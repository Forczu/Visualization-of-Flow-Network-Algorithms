class FordFulkersonAlgorithm : public FlowNetworkAlgorithm
{
public:
	int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork) override;
	QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) override;
	void increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase) override;
};

int FordFulkersonAlgorithm::makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork)
{
	return FlowNetworkAlgorithm::makeResidualNetwork(network, residualNewtork);
}

QList<EdgeImage*> FordFulkersonAlgorithm::findAugumentingPath(FlowNetwork * residualNetwork, int & capacity)
{
	return FlowNetworkAlgorithm::findAugumentingPath(residualNetwork, capacity);
}

void FordFulkersonAlgorithm::increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase)
{
	FlowNetworkAlgorithm::increaseFlow(network, path, increase);
}