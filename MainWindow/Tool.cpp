#include "Tool.h"

Tool * DrawingTool::_tool = 0;

void DrawingTool::changeTool(Tool * tool)
{
	_tool = tool;
}

void DrawingTool::mousePressed(GraphView * graph, QPointF const & pos, QList<QGraphicsItem*> const & items)
{
	_tool->handleMousePress(graph, pos, items);
}

void DrawingTool::mouseMoved(GraphView * graph, QPoint const & position)
{
	_tool->handleMouseMove(graph, position);
}

void DrawingTool::mouseReleased(GraphView * graph)
{
	_tool->handleMouseRelease(graph);
}
