#pragma once
#include <QMap>
#include "IAlgorithm.h"
#include "FlowNetworkAlgorithmState.h"
#include "AlgorithmState.h"
#include "Orders.h"


class AlgorithmInfo
{
	static AlgorithmState * _state;
	friend class AlgorithmState;
public:
	void changeState(Order order);
	IAlgorithm * getAlgorithm(QString const & name);
	QStringList getAlgorithmList();
};

