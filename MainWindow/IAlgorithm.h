#pragma once
#include "GraphImage.h"

class IAlgorithm
{
public:
	virtual void run(GraphImage * graph) = 0;
};

