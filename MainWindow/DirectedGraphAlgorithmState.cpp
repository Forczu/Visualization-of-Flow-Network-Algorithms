#include "DirectedGraphAlgorithmState.h"

DirectedGraphAlgorithmState * DirectedGraphAlgorithmState::_pInstance = 0;
char const * DirectedGraphAlgorithmState::FLOYD_WARSHALL = "floyd_warshall";
char const * DirectedGraphAlgorithmState::DIJKSTRA = "dijkstra";

DirectedGraphAlgorithmState::DirectedGraphAlgorithmState()
{
	_map = getAlgorithmMap();
}

QStringList DirectedGraphAlgorithmState::getAlgorithmList()
{
	return _map.values();
}

QMap<QString, QString> DirectedGraphAlgorithmState::getAlgorithmMap()
{
	return Application::Config::Instance().getAlgorithmMap("directed_graph");
}

IAlgorithm * DirectedGraphAlgorithmState::getAlgorithm(QString const & name)
{
	QString algName = _map.key(name);
	if (algName == FLOYD_WARSHALL)
	{
		return nullptr;
	}
}

QDialog * DirectedGraphAlgorithmState::getDialog(GraphImage * graph, QString const & name)
{
	return nullptr;
}
