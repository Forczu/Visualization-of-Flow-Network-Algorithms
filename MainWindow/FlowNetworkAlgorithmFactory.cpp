#include "FlowNetworkAlgorithmFactory.h"

FlowNetworkAlgorithmFactory * FlowNetworkAlgorithmFactory::_pInstance = 0;

const bool FlowNetworkAlgorithmFactory::registerAlgorithm(QString const & name, CreateAlgorithmCallback callback)
{
	return _callbackMap.insert(name, callback) != _callbackMap.end();
}

bool FlowNetworkAlgorithmFactory::unregisterAlgorithm(QString const & name)
{
	return _callbackMap.remove(name) == 1;
}

FlowNetworkAlgorithm * FlowNetworkAlgorithmFactory::createAlgorithm(QString const & name)
{
	CallbackMap::const_iterator it = _callbackMap.find(name);
	if (it == _callbackMap.end())
		throw std::runtime_error("Nieznany identyfikator algorytmu");
	return (*it)();
}
