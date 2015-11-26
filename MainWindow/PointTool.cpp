#include "PointTool.h"
#include <QGraphicsItem>

PointTool * PointTool::_pInstance = 0;

void PointTool::handleMousePress(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items, Qt::KeyboardModifiers const & modifiers)
{
	if (items.size() == 0)
		graph->startRubberBand(pos);
}

void PointTool::handleMouseRelease(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items, Qt::KeyboardModifiers const & modifiers)
{
}
