#include "RemoveTool.h"

RemoveTool * RemoveTool::_pInstance = 0;

void RemoveTool::handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items)
{
	graph->remove(items);
}
