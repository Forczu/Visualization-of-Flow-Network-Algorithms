#include "PointTool.h"

PointTool * PointTool::_pInstance = 0;

void PointTool::handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items)
{
	graph->pointItem(pos, items);
}
