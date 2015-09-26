#include "GraphView.h"

GraphView::GraphView() : _mouseClicked(false), _rubberFlag(false)
{
	init();
}


GraphView::GraphView(QWidget * widget) : QGraphicsView(widget), _mouseClicked(false)
{
	init();
}

void GraphView::init()
{
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
	if (std::abs(coord.first.x() - coord.second.x()) <= size &&
		std::abs(coord.first.y() - coord.second.y()) <= size)
	{
		item = new LoopEdgeImage(_vertexMap[edge->VertexFrom()->Id()], _vertexMap[edge->VertexTo()->Id()]);
	}
	else
	{
		item = new StraightEdgeImage(_vertexMap[edge->VertexFrom()->Id()], _vertexMap[edge->VertexTo()->Id()]);
	}
	item->setEdge(edge);
	item->setFlag(QGraphicsItem::ItemIsMovable, false);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	item->setZValue(EDGE_Z_VALUE);
	scene()->addItem(item);
	scene()->update();
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

void GraphView::grabItem(QList<QGraphicsItem*> const & items)
{
	if (items.size() == 0)
	{
		unselectAll();
		return;
	}
	for each (QGraphicsItem* item in items)
	{
		if (item->isSelected())
			continue;
		item->setSelected(true);
	}
	//unselectAll(item);
}

void GraphView::startRubberBand(QPoint const & position)
{
	_origin = position;
	if (_rubberBand == nullptr)
		_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	_rubberFlag = true;
	_rubberBand->setGeometry(QRect(_origin, QSize()));
	_rubberBand->show();
}

void GraphView::mousePressEvent(QMouseEvent * event)
{
	auto chosenItems = items(event->pos());
	emit clicked(event->pos(), chosenItems);
	viewport()->update();
	QGraphicsView::mousePressEvent(event);
}

void GraphView::mouseReleaseEvent(QMouseEvent * event)
{
	if (_rubberBand != nullptr && _rubberFlag)
	{
		_rubberBand->hide();
		viewport()->update();
		_rubberFlag = false;
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void GraphView::mouseMoveEvent(QMouseEvent * event)
{
	if (_rubberBand != nullptr && _rubberFlag)
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
