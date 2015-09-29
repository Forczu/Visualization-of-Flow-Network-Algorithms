#include "GraphView.h"

GraphView::GraphView()
{
	init();
}


GraphView::GraphView(QWidget * widget) : QGraphicsView(widget)
{
	init();
}

void GraphView::init()
{
	_mouseClicked = false;
	_toolFlag = Tool::None;
	_rubberFlag = false;
	_grabFlag = false;
	_rubberBand = nullptr;
	setMouseTracking(true);
	viewport()->setMouseTracking(true);
}

void GraphView::unselectAll(QGraphicsItem * const except)
{
	if (except == nullptr)
	{
		for each (QGraphicsItem* item in scene()->items())
		{
			item->setSelected(false);
		}
	}
	else
	{
		for each (QGraphicsItem* item in scene()->items())
		{
			if (item != except)
				item->setSelected(false);
		}
	}
}

void GraphView::changeSelection()
{

}

GraphView::~GraphView()
{
}

void GraphView::addVertexImage(Vertex * const vertex, QPoint const & position)
{
	VertexImage * item = new VertexImage(Application::Config::Instance().DefaultVertexContext());
	item->setVertex(vertex);
	item->setPos(mapToScene(position));
	item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	item->setZValue(VERTICE_Z_VALUE);
	scene()->addItem(item);
	scene()->update();
	_vertexMap[vertex->Id()] = item;
}

void GraphView::addEdgeImage(Edge * const edge, std::pair<int, int> const & pair, std::pair<QPointF, QPointF> const & coord)
{
	int size = Application::Config::Instance().DefaultVertexContext().Size();
	EdgeImage * item;
	VertexImage * vertexFrom = _vertexMap[edge->VertexFrom()->Id()];
	VertexImage * vertexTo = _vertexMap[edge->VertexTo()->Id()];
	if (std::abs(coord.first.x() - coord.second.x()) <= size &&
		std::abs(coord.first.y() - coord.second.y()) <= size)
	{
		item = new LoopEdgeImage(vertexFrom, vertexTo, Application::Config::Instance().DefaultEdgeContext());
	}
	else
	{
		item = new StraightEdgeImage(vertexFrom, vertexTo, Application::Config::Instance().DefaultEdgeContext());
	}
	item->setEdge(edge);
	item->setFlag(QGraphicsItem::ItemIsMovable, false);
	item->setFlag(QGraphicsItem::ItemIsSelectable, false);
	item->setZValue(EDGE_Z_VALUE);
	scene()->addItem(item);
	scene()->update();
	_edgeMap[std::make_pair(edge->VertexFrom()->Id(), edge->VertexTo()->Id())] = item;
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

void GraphView::grabItem(QPoint const & position)
{
	setCursor(Qt::ClosedHandCursor);
	_offset = mapToScene(position).toPoint();
	_grabFlag = true;
}

void GraphView::pointItem(QPoint const & position, QList<QGraphicsItem*> const & items)
{
	if (items.size() == 0)
	{
		startRubberBand(position);
	}
	else if (items.first()->isSelected())
	{
		grabItem(position);
	}
	else
	{
		unselectAll();
		for each (QGraphicsItem* item in items)
		{
			item->setSelected(true);
		}
	}
	viewport()->update();
}

void GraphView::startRubberBand(QPoint const & position)
{
	unselectAll();
	_origin = position;
	if (_rubberBand == nullptr)
		_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	_rubberBand->setGeometry(QRect(_origin, QSize()));
	_rubberBand->show();
	_rubberFlag = true;
}

void GraphView::setTool(Tool tool)
{
	_toolFlag = tool;
}

void GraphView::removeItem(QGraphicsItem * item)
{
	scene()->removeItem(item);
	delete item;
}

void GraphView::removeEdge(EdgeImage * const edge)
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); ++it)
	{
		EdgeImage * item = (*it).second;
		if (edge == item)
		{
			removeItem(edge);
			it = _edgeMap.erase(it);
			break;
		}
	}
}

void GraphView::removeVertex(VertexImage * const vertex)
{
	for (EdgeImageMap::iterator it = _edgeMap.begin(); it != _edgeMap.end(); )
	{
		EdgeImage * edge = (*it).second;
		if (edge->VertexFrom() == vertex || edge->VertexTo() == vertex)
		{
			removeItem(edge);
			it = _edgeMap.erase(it);
		}
		else
			++it;
	}
	removeItem(vertex);
}

void GraphView::mousePressEvent(QMouseEvent * event)
{
	auto chosenItems = items(event->pos());
	emit clicked(event->pos(), chosenItems);
	QGraphicsView::mousePressEvent(event);
	viewport()->update();
}

void GraphView::mouseMoveEvent(QMouseEvent * event)
{
	//if (_grabFlag)
	//{
	//	auto selectedItems = scene()->selectedItems();
	//	QPoint mappedMousePos = mapToScene(event->localPos().toPoint()).toPoint();
	//	QPoint delta = mappedMousePos - _offset;
	//	for each (QGraphicsItem* item in selectedItems)
	//	{
	//		item->setPos(item->scenePos() + delta);
	//	}
	//	_offset = mappedMousePos;
	//}
	/*else*/ if (_rubberBand != nullptr && _rubberFlag)
	{
		_rubberBand->setGeometry(QRect(_origin, event->pos()).normalized());
		QRect rubberRect = mapRubberBandToScene();
		auto items = scene()->items();
		for each (QGraphicsItem* item in items)
		{
			QRect itemRect = item->mapRectToScene(item->boundingRect()).toRect();
			if (itemRect.intersects(rubberRect))
				item->setSelected(true);
			else
				item->setSelected(false);
		}
	}
	QGraphicsView::mouseMoveEvent(event);
	viewport()->update();
}

void GraphView::mouseReleaseEvent(QMouseEvent * event)
{
	if (_grabFlag)
	{
		_grabFlag = false;
		setCursor(Qt::OpenHandCursor);
	}
	else if (_rubberBand != nullptr && _rubberFlag)
	{
		_rubberBand->hide();
		viewport()->update();
		_rubberFlag = false;
	}
	QGraphicsView::mouseReleaseEvent(event);
}

QRect GraphView::mapRubberBandToScene()
{
	QRect rubberRect = _rubberBand->geometry();
	QPointF p1 = mapToScene(QPoint(rubberRect.left(), rubberRect.top()));
	QPointF p2 = mapToScene(QPoint(rubberRect.right(), rubberRect.bottom()));
	rubberRect.setCoords(p1.x(), p1.y(), p2.x(), p2.y());
	return rubberRect;
}

void GraphView::resizeEvent(QResizeEvent * event)
{
	//fitInView(sceneRect(), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}

void GraphView::showEvent(QShowEvent * event)
{
	// wywo³ywane tylko w momencie uruchomienia
	QPoint startPoint = pos();
	int sceneWidth = width();
	int sceneHeight = height();
	QRectF sceneRect = QRectF(startPoint.x(), startPoint.y(), sceneWidth, sceneHeight);
	scene()->setSceneRect(sceneRect);
	QGraphicsView::showEvent(event);
}
