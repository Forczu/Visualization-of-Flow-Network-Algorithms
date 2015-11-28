#pragma once
#include <QMap>
#include "IAlgorithm.h"
#include "FlowNetworkAlgorithmState.h"
#include "AlgorithmState.h"

class AlgorithmInfo
{
	static AlgorithmState * _state;
	friend class AlgorithmState;
public:
	void changeState(GraphImage * graph);
	IAlgorithm * getAlgorithm(QString const & name);
	QStringList getAlgorithmList();
	QDialog * getDialog(GraphImage * graph, QString const & name);
	CheckInfo checkGraph(GraphImage * graph);
};

