#include "FlowNetworkAlgorithmState.h"
#include "Config.h"
#include "FordFulkersonAlgorithm.h"
#include "CheckInfo.h"
#include "Strings.h"
#include "FlowNetworkAlgorithmWindow.h"
#include "DinicAlgorithm.h"
#include "BlockingFlowAlgorithmWindow.h"
#include "MkmAlgorithm.h"

FlowNetworkAlgorithmState * FlowNetworkAlgorithmState::_pInstance = 0;

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

FlowNetworkAlgorithm * FlowNetworkAlgorithmState::getAlgorithm(QString const & name)
{
	try
	{
		return FlowNetworkAlgorithmFactory::Instance().createAlgorithm(_map.key(name));
	}
	catch (std::runtime_error const & e)
	{
		return nullptr;
	}
}

QDialog * FlowNetworkAlgorithmState::getDialog(GraphImage * graph, QString const & name)
{
	FlowNetworkAlgorithm * algorithm = dynamic_cast<FlowNetworkAlgorithm*>(getAlgorithm(name));
	FlowNetwork * network = dynamic_cast<FlowNetwork*>(graph);
	FlowNetworkAlgorithmWindow * dialog;
	if (dynamic_cast<FordFulkerson::FordFulkersonAlgorithm*>(algorithm))
		dialog = new FlowNetworkAlgorithmWindow(network, algorithm);
	else
		dialog = new BlockingFlowAlgorithmWindow(network, algorithm);
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
	if (network->getSourceId() == 0)
	{
		addErrorMessage(info, WRONG_SOURCE);
		info.setSucceeded(false);
	}
	if (network->getTargetId() == 0)
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
		info.setSucceeded(false);
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
