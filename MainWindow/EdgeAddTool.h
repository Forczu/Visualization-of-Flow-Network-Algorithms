#pragma once
#include "Tool.h"

class EdgeAddTool : public Tool, public Singleton<EdgeAddTool>
{
public:
	void handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items);
	void handleMouseMove(GraphView * graph, QPoint const & position);
	void handleMouseRelease(GraphView * graph);
};

