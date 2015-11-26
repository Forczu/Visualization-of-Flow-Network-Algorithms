#include "EdgeAddTool.h"

EdgeAddTool * EdgeAddTool::_pInstance = 0;

void EdgeAddTool::handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items, Qt::KeyboardModifiers const & modifiers)
{
	if (items.size() == 0)
		return;
	graph->buildEdge(items.first());
}

void EdgeAddTool::handleMouseMove(GraphView * graph, QPoint const & position)
{
	graph->changeVerticesLabels(position);
}