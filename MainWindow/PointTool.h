#pragma once
#include "Tool.h"

class PointTool : public Tool, public Singleton<PointTool>
{
public:
	void handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items);
};

