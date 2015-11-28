#pragma once
#include "Singleton.h"
#include <QString>
#include <QMap>
#include <vector>

static const char * ALGORITHM_ERROR = "ALGORITHM_ERROR";
static const char * ERROR_MAIN_INFO = "ERROR_MAIN_INFO";
static const char * WRONG_SOURCE = "WRONG_SOURCE";
static const char * WRONG_TARGET = "WRONG_TARGET";
static const char * EMPTY_NETWORK = "EMPTY_NETWORK";
static const char * FLOW_GREATER_THAN_CAPACITY = "FLOW_GREATER_THAN_CAPACITY";
static const char * NO_ROUTE_TO_TARGET = "NO_ROUTE_TO_TARGET";
static const char * SUMS_OF_INFLOWS_AND_OUTFLOWS_NOT_EQUAL = "SUMS_OF_INFLOWS_AND_OUTFLOWS_NOT_EQUAL";

class Strings : public Singleton<Strings>
{
	QMap<QString, QString> _map;
public:
	Strings();

	QString get(QString const & key)
	{
		return _map[key];
	}

	QString get(std::string const & key)
	{
		return _map[QString::fromStdString(key)];
	}

	QString get(const char * key)
	{
		return _map[key];
	}

	void add(QString const & key, QString const & value)
	{
		Instance()._map[key] = value;
	}
};

