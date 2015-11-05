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
	return Application::Config::Instance().getAlgorithmMap(Order::FlowNetwork);
}

IAlgorithm * FlowNetworkAlgorithmState::getAlgorithm(QString const & name)
{
	QString algName = _map.key(name);
	if (algName == FORD_FULKERSON)
	{
		return FordFulkersonAlgorithm::getInstance();
	}
}
