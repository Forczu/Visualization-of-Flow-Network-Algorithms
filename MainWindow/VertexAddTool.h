#pragma once
#include "Tool.h"

class VertexAddTool : public Tool, public Singleton<VertexAddTool>
{
public:
	void handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items) override;
};