#pragma once
#include "DirectedGraphImage.h"

class FlowNetwork :	public DirectedGraphImage
{
private:
	int _source;
	int _target;
	TextItem * _sourceLabel;
	TextItem * _targetLabel;

public:
	FlowNetwork(GraphConfig * config);
	~FlowNetwork();
};

