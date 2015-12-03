#include "AlgorithmInfo.h"
#include "FlowNetwork.h"
#include "FlowNetworkAlgorithmState.h"

AlgorithmState * AlgorithmInfo::_state = 0;

void AlgorithmInfo::changeState(GraphImage * graph)
{
	if (dynamic_cast<FlowNetwork*>(graph) != NULL)
		_state = &FlowNetworkAlgorithmState::Instance();
}

FlowNetworkAlgorithm * AlgorithmInfo::getAlgorithm(QString const & name)
{
	return _state->getAlgorithm(name);
}

QStringList AlgorithmInfo::getAlgorithmList()
{
	return _state->getAlgorithmList();
}


QDialog * AlgorithmInfo::getDialog(GraphImage * graph, QString const & name)
{
	return _state->getDialog(graph, name);
}

CheckInfo AlgorithmInfo::checkGraph(GraphImage * graph)
{
	return _state->checkGraph(graph);
}
