#pragma once
#include "IAlgorithm.h"
#include <QMap>

class AlgorithmState
{
protected:
	QMap<QString, QString> _map;
public:
	virtual QStringList getAlgorithmList() = 0;
	virtual QMap<QString, QString> getAlgorithmMap() = 0;
	virtual IAlgorithm * getAlgorithm(QString const & name) = 0;
	virtual QDialog * getDialog(GraphImage * graph, QString const & name) = 0;
};
