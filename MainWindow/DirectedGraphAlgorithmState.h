#pragma once
#include "AlgorithmState.h"
#include "Singleton.h"
#include "Config.h"

class DirectedGraphAlgorithmState : public AlgorithmState, public Singleton<DirectedGraphAlgorithmState>
{
protected:
	static const char * FLOYD_WARSHALL;
	static const char * DIJKSTRA;
public:
	DirectedGraphAlgorithmState();

	QStringList getAlgorithmList() override;

	QMap<QString, QString> getAlgorithmMap() override;

	IAlgorithm * getAlgorithm(QString const & name) override;

	QDialog * getDialog(GraphImage * graph, QString const & name) override;

	CheckInfo checkGraph(GraphImage * graph) override;

};

