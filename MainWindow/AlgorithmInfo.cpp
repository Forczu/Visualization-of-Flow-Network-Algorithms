#include "AlgorithmInfo.h"

AlgorithmState * AlgorithmInfo::_state = 0;

void AlgorithmInfo::changeState(Order order)
{
	switch (order)
	{
	case Order::FlowNetwork:
		_state = &FlowNetworkAlgorithmState::Instance();
	}
}

IAlgorithm * AlgorithmInfo::getAlgorithm(QString const & name)
{
	return _state->getAlgorithm(name);
}

QStringList AlgorithmInfo::getAlgorithmList()
{
	return _state->getAlgorithmList();
}
