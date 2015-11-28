#include "FlowNetworkAlgorithmState.h"
#include "Config.h"
#include "FordFulkersonAlgorithm.h"
#include "CheckInfo.h"
#include "Strings.h"

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

CheckInfo FlowNetworkAlgorithmState::checkGraph(GraphImage * graph)
{
	CheckInfo info(true);
	FlowNetwork * network = dynamic_cast<FlowNetwork*>(graph);
	if (network == NULL)
	{
		addErrorMessage(info, EMPTY_NETWORK);
		info.setSucceeded(false);
	}
	if (network->getSource() == 0)
	{
		addErrorMessage(info, WRONG_SOURCE);
		info.setSucceeded(false);
	}
	if (network->getTarget() == 0)
	{
		addErrorMessage(info, WRONG_TARGET);
		info.setSucceeded(false);
	}
	if (!network->checkCapacityCondition(info))
	{
		info.setSucceeded(false);
	}
	if (!network->checkFlowPreservation(info))
	{

	}
	if (!network->checkStructure(info))
	{
		info.setSucceeded(false);
	}
	return info;
}

void FlowNetworkAlgorithmState::addErrorMessage(CheckInfo &info, const char *code)
{
	info += Strings::Instance().get(code);
}
