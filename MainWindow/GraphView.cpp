#include "GraphView.h"

GraphView::GraphView() : _mouseClicked(false)
{
	QPoint startPoint = pos();
	int sceneWidth = width();
	int sceneHeight = height();
	_graphScene = new QGraphicsScene;
	setScene(_graphScene);
}


GraphView::GraphView(QWidget * widget) : QGraphicsView(widget), _mouseClicked(false)
{
	QPoint startPoint = pos();
	int sceneWidth = width();
	int sceneHeight = height();
	_graphScene = new QGraphicsScene(startPoint.x(), startPoint.y(), sceneWidth, sceneHeight);
	setScene(_graphScene);
}

GraphView::~GraphView()
{
}

void GraphView::wheelEvent(QWheelEvent * event)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	// Scale the view / do the zoom
	double scaleFactor = 1.15;
	if (event->delta() > 0) {
		// Zoom in
		scale(scaleFactor, scaleFactor);
	}
	else {
		// Zooming out
		scale(1.0 / scaleFactor, 1.0 / scaleFactor);
	}
}

void GraphView::mousePressEvent(QMouseEvent * event)
{
	auto chosenItems = items(event->pos());
	emit clicked(event->pos(), chosenItems);
	QGraphicsView::mousePressEvent(event);
	if (chosenItems.size() == 0)
		return;
	VerticeImage * myVertice = dynamic_cast<VerticeImage*>(chosenItems.first());
	if (NULL != myVertice)
	{
		bool selected = !myVertice->isSelected();
		myVertice->select(selected);
		viewport()->update();
	}
}

void GraphView::mouseReleaseEvent(QMouseEvent * event)
{
	QGraphicsView::mouseReleaseEvent(event);
}

void GraphView::mouseMoveEvent(QMouseEvent * event)
{
	QGraphicsView::mouseMoveEvent(event);
	viewport()->update();
}
