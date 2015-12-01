#include "ZoomTool.h"
ZoomTool * ZoomTool::_pInstance = 0;

void ZoomTool::handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items, Qt::KeyboardModifiers const & modifiers)
{
	graph->zoom(pos, modifiers);
}
