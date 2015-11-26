#pragma once
#include "Tool.h"

class EdgeAddTool : public Tool, public Singleton<EdgeAddTool>
{
public:
	void handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items, Qt::KeyboardModifiers const & modifiers);
	void handleMouseMove(GraphView * graph, QPoint const & position);
};

