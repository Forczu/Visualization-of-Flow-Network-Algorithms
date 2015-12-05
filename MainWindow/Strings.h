#pragma once
#include "Singleton.h"
#include <QMap>

static const char * ALGORITHM_ERROR = "ALGORITHM_ERROR";
static const char * ERROR_MAIN_INFO = "ERROR_MAIN_INFO";
static const char * WRONG_SOURCE = "WRONG_SOURCE";
static const char * WRONG_TARGET = "WRONG_TARGET";
static const char * EMPTY_NETWORK = "EMPTY_NETWORK";
static const char * FLOW_GREATER_THAN_CAPACITY = "FLOW_GREATER_THAN_CAPACITY";
static const char * NO_ROUTE_TO_TARGET = "NO_ROUTE_TO_TARGET";
static const char * SUMS_OF_INFLOWS_AND_OUTFLOWS_NOT_EQUAL = "SUMS_OF_INFLOWS_AND_OUTFLOWS_NOT_EQUAL";
static const char * VERTEX_TOOL_TIP_1 = "VERTEX_TOOL_TIP_1";
static const char * VERTEX_TOOL_TIP_2 = "VERTEX_TOOL_TIP_2";
static const char * SOURCE = "SOURCE";
static const char * TARGET = "TARGET";
static const char * WRONG_VALUE = "WRONG_VALUE";
static const char * FLOW_GREATER_THAN_CAPACITY_MSGBX = "FLOW_GREATER_THAN_CAPACITY_MSGBX";
static const char * VERTEX_WITH_ID = "VERTEX_WITH_ID";
static const char * GRAPH_STATUS = "GRAPH_STATUS";
static const char * MARK_SOURCE = "MARK_SOURCE";
static const char * MARK_TARGET = "MARK_TARGET";
static const char * FLOW_NETWORK_RESIDUAL_CREATED = "FLOW_NETWORK_RESIDUAL_CREATED";
static const char * FLOW_NETWORK_AUGUMENTING_PATH_FOUND = "FLOW_NETWORK_AUGUMENTING_PATH_FOUND";
static const char * FLOW_NETWORK_FLOW_INCREASED = "FLOW_NETWORK_FLOW_INCREASED";
static const char * FLOW_NETWORK_ALGORITHM_FINISHED = "FLOW_NETWORK_ALGORITHM_FINISHED";
static const char * FLOW_NETWORK_FULL_SOURCE_CAPACITY = "FLOW_NETWORK_FULL_SOURCE_CAPACITY";
static const char * FLOW_NETWORK_MAX_FLOW = "FLOW_NETWORK_MAX_FLOW";
static const char * FLOW_NETWORK_AUGUMENTING_PATH_NOT_FOUND = "FLOW_NETWORK_AUGUMENTING_PATH_NOT_FOUND";
static const char * FLOW_NETWORK_FULL_TARGET_CAPACITY = "FLOW_NETWORK_FULL_TARGET_CAPACITY";
static const char * FORD_FULKERSON_ALGORITHM = "FORD_FULKERSON_ALGORITHM";
static const char * DINIC_ALGORITHM = "DINIC_ALGORITHM";
static const char * MKM_ALGORITHM = "MKM_ALGORITHM";
static const char * LAYERED_RESIDUAL_NETWORK_CREATED = "LAYERED_RESIDUAL_NETWORK_CREATED";
static const char * BLOCKING_FLOW_CREATED = "BLOCKING_FLOW_CREATED";
static const char * BLOCKING_FLOW_AUGUMENTING_PATH = "BLOCKING_FLOW_AUGUMENTING_PATH";
static const char * BLOCKING_FLOW_STARTED = "BLOCKING_FLOW_STARTED";
static const char * RESIDUAL_NETWORK = "RESIDUAL_NETWORK";
static const char * FLOW_NETWORK = "FLOW_NETWORK";
static const char * BLOCKING_FLOW = "BLOCKING_FLOW";
static const char * RESIDUAL_NETWORK_FLOW_INCREASED = "RESIDUAL_NETWORK_FLOW_INCREASED";
static const char * NULL_DEGREE_VERTICES_REMOVED = "NULL_DEGREE_VERTICES_REMOVED";
static const char * BLOCKING_FLOW_FOUND = "BLOCKING_FLOW_FOUND";
static const char * FLOW_NETWORK_RESTORED = "FLOW_NETWORK_RESTORED";
static const char * NULL_POTENTIAL_VERTICES_REMOVED = "NULL_POTENTIAL_VERTICES_REMOVED";
static const char * OPEN_GRAPH_FILE = "OPEN_GRAPH_FILE";
static const char * SAVE_GRAPH_TO_FILE = "SAVE_GRAPH_TO_FILE";

class Strings : public Singleton<Strings>
{
	/// <summary>
	/// Mapa, która zawiera klucze oraz stringi jaki przechowuj¹
	/// </summary>
	QMap<QString, QString> _map;
public:
	explicit Strings();

	inline QString get(QString const & key) { return _map[key]; }

	inline QString get(std::string const & key) { return _map[QString::fromStdString(key)]; }

	inline QString get(const char * key) { return _map[key]; }

	void add(QString const & key, QString const & value);
};

