#pragma once
#include "AlgorithmState.h"
#include "Singleton.h"
#include "FlowNetworkAlgorithmFactory.h"

class FlowNetworkAlgorithmState : public AlgorithmState, public Singleton<FlowNetworkAlgorithmState>
{
public:
	explicit FlowNetworkAlgorithmState();
	QStringList getAlgorithmList() override;
	QMap<QString, QString> getAlgorithmMap() override;
	FlowNetworkAlgorithm * getAlgorithm(QString const & name) override;
	QDialog * getDialog(GraphImage * graph, QString const & name) override;
	CheckInfo checkGraph(GraphImage * graph) override;
private:
	void addErrorMessage(CheckInfo &info, const char *code);

};

