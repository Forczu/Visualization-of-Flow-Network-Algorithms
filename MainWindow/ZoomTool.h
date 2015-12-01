#pragma once
#include "Tool.h"

class ZoomTool : public Tool, public Singleton<ZoomTool>
{
public:
	void handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items, Qt::KeyboardModifiers const & modifiers) override;
};
