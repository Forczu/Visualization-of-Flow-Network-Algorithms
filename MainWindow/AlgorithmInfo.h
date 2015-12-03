#pragma once
#include <QMap>
#include "AlgorithmState.h"

class FlowNetworkAlgorithm;
class QDialog;
class GraphImage;

class AlgorithmInfo
{
	static AlgorithmState * _state;
	friend class AlgorithmState;
public:
	void changeState(GraphImage * graph);
	FlowNetworkAlgorithm * getAlgorithm(QString const & name);
	QStringList getAlgorithmList();
	QDialog * getDialog(GraphImage * graph, QString const & name);
	CheckInfo checkGraph(GraphImage * graph);
};

