#pragma once
#include <QMap>
#include "Singleton.h"

class FlowNetworkAlgorithm;

/// <summary>
/// Fabryka obiektów algorytmów dla sieci przep³ywowych.
/// </summary>
class FlowNetworkAlgorithmFactory : public Singleton<FlowNetworkAlgorithmFactory>
{
	typedef FlowNetworkAlgorithm* (*CreateAlgorithmCallback)();
	typedef QMap<QString, CreateAlgorithmCallback> CallbackMap;
	CallbackMap _callbackMap;
public:
	const bool registerAlgorithm(QString const & name, CreateAlgorithmCallback callback);
	bool unregisterAlgorithm(QString const & name);
	FlowNetworkAlgorithm * createAlgorithm(QString const & name);
};
