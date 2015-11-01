#pragma once
#include "DirectedGraphImage.h"
#include <map>
class FlowNetwork :	public DirectedGraphImage
{
private:
	int _source;
	int _target;
	TextItem * _sourceLabel;
	TextItem * _targetLabel;
	std::map<int, int> _flowMap;

public:
	FlowNetwork(GraphConfig * config);
	~FlowNetwork();
};

