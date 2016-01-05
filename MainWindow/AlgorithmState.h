#pragma once
#include <QMap>
#include "CheckInfo.h"

class GraphImage;
class FlowNetworkAlgorithm;
class QDialog;

/// <summary>
/// Okreœla aktualny stan algorytmu, jaki jest wykonywany.
/// </summary>
class AlgorithmState
{
protected:
	QMap<QString, QString> _map;
public:
	virtual QStringList getAlgorithmList() = 0;
	virtual QMap<QString, QString> getAlgorithmMap() = 0;
	virtual FlowNetworkAlgorithm * getAlgorithm(QString const & name) = 0;
	virtual QDialog * getDialog(GraphImage * graph, QString const & name) = 0;
	virtual CheckInfo checkGraph(GraphImage * graph) = 0;
};
