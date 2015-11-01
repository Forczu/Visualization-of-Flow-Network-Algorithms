#pragma once
#include "Tool.h"

class RemoveTool : public Tool, public Singleton<RemoveTool>
{
public:
	void handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items);
};

