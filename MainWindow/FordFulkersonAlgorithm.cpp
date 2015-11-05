#include "FordFulkersonAlgorithm.h"
#include "FlowNetworkAlgorithmWindow.h"

FordFulkersonAlgorithm * FordFulkersonAlgorithm::getInstance()
{
	return new FordFulkersonAlgorithm;
}

void FordFulkersonAlgorithm::run(GraphImage * graph)
{
	FlowNetworkAlgorithmWindow * window = new FlowNetworkAlgorithmWindow;
	window->setMainNetwork(new FlowNetwork(*dynamic_cast<FlowNetwork*>(graph)));
	window->setAlgorithm(this);
	window->exec();
}

FlowNetwork * FordFulkersonAlgorithm::makeResidualNetwork()
{
	return nullptr;
}

QList<int> FordFulkersonAlgorithm::findAugumentingPath()
{
	return QList<int>();
}

void FordFulkersonAlgorithm::increaseFlow(QList<int> const & path)
{

}
