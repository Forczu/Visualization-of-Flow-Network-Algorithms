#pragma once
#include "AlgorithmState.h"
#include "Singleton.h"

class FlowNetworkAlgorithmState : public AlgorithmState, public Singleton<FlowNetworkAlgorithmState>
{
protected:
	static char const * FORD_FULKERSON;
	static const char * DINIC;
	static const char * MKM;
public:
	FlowNetworkAlgorithmState();
	QStringList getAlgorithmList() override;
	QMap<QString, QString> getAlgorithmMap() override;
	FlowNetworkAlgorithm * getAlgorithm(QString const & name) override;
	QDialog * getDialog(GraphImage * graph, QString const & name) override;
	CheckInfo checkGraph(GraphImage * graph) override;
private:
	void addErrorMessage(CheckInfo &info, const char *code);

};

