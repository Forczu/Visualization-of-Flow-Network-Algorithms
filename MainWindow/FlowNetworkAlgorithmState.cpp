#include "FlowNetworkAlgorithmState.h"
#include "Config.h"
#include "FordFulkersonAlgorithm.h"

FlowNetworkAlgorithmState * FlowNetworkAlgorithmState::_pInstance = 0;
char const * FlowNetworkAlgorithmState::FORD_FULKERSON = "ford_fulkerson";
char const * FlowNetworkAlgorithmState::DINIC = "dinic";
char const * FlowNetworkAlgorithmState::MKM = "mkm";

FlowNetworkAlgorithmState::FlowNetworkAlgorithmState()
{
	_map = getAlgorithmMap();
}

QStringList FlowNetworkAlgorithmState::getAlgorithmList()
{
	return _map.values();
}

QMap<QString, QString> FlowNetworkAlgorithmState::getAlgorithmMap()
{
	return Application::Config::Instance().getAlgorithmMap("flow_network");
}

IAlgorithm * FlowNetworkAlgorithmState::getAlgorithm(QString const & name)
{
	QString algName = _map.key(name);
	if (algName == FORD_FULKERSON)
	{
		return FordFulkersonAlgorithm::getInstance();
	}
}

QDialog * FlowNetworkAlgorithmState::getDialog(GraphImage * graph, QString const & name)
{
	FlowNetworkAlgorithmWindow * dialog = new FlowNetworkAlgorithmWindow;
	FlowNetworkAlgorithm * algorithm = dynamic_cast<FlowNetworkAlgorithm*>(getAlgorithm(name));
	FlowNetwork * network = dynamic_cast<FlowNetwork*>(graph);
	dialog->setAlgorithm(algorithm);
	dialog->scaleViews(network);
	return dialog;
}

bool FlowNetworkAlgorithmState::checkGraph(GraphImage * graph)
{
	FlowNetwork * network = dynamic_cast<FlowNetwork*>(graph);
	if (network == NULL)
		return false;
	if (network->getSource() == 0)
		return false;
	if (network->getTarget() == 0)
		return false;
	if (!network->checkCapacityCondition())
		return false;
	if (!network->checkFlowPreservation())
		return false;
	if (!network->checkStructure())
		return false;
	return true;
}
