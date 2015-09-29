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
	_graphScene = new QGraphicsScene;
	_graphScene->setSceneRect(QRect(-2000, -2000, 4000, 4000));
	setScene(_graphScene);

	_mouseClicked = false;
	_toolFlag = Tool::None;
	_rubberFlag = false;
	_grabFlag = false;
	_rubberBand = nullptr;
	_edgeFlag = EdgeFlag::None;
	setMouseTracking(true);
	viewport()->setMouseTracking(true);

	_labelMap["source"] = new TextItem("Source");
	_labelMap["target"] = new TextItem("Target");
	scene()->addItem(_labelMap["source"]);
	scene()->addItem(_labelMap["target"]);

	QFont font;
	font.setBold(true);
	font.setItalic(true);
	font.setPointSize(16);
	font.setFamily(QString("Calibri"));
	_labelMap["source"]->replaceFont(font);
	_labelMap["target"]->replaceFont(font);
	_labelMap["source"]->setAlignment(Qt::AlignLeft);
	_labelMap["target"]->setAlignment(Qt::AlignRight);
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

void GraphView::addVertexImage(VertexPtr const & vertex, QPoint const & position)
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
	const double scaleFactor = 1.15;
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
	QPointF point(mapToScene(event->pos()));
	auto item = scene()->itemAt(point, transform());
	VertexImage * img = dynamic_cast<VertexImage*>(item);
	if (NULL != img)
	{
		switch (_edgeFlag)
		{
		case EdgeFlag::Source:
			_labelMap["source"]->setBoundingRect(QRect(
				img->scenePos().x() - img->Context()->Size(),
				img->scenePos().y() - img->Context()->Size() - 40,
				img->Context()->Size() * 2,
				40));
			break;
		case EdgeFlag::Target:
			_labelMap["target"]->setBoundingRect(QRect(
				img->scenePos().x() - img->Context()->Size(),
				img->scenePos().y() - img->Context()->Size() - 40,
				img->Context()->Size() * 2,
				40));
			break;
		default:
			std::for_each(_labelMap.begin(), _labelMap.end(), [](std::pair<std::string, TextItem*> label)
			{
				label.second->setBoundingRect(QRect());
			});
			break;
		}
	}
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
