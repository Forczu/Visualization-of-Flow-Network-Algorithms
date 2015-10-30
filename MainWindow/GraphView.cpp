#include "GraphView.h"
#include "GraphScrollBar.h"
#include "VertexImage.h"
#include "EdgeImage.h"
#include "LoopEdgeImage.h"
#include "StraightEdgeImage.h"
#include "TextItem.h"
#include "ArrowHeadImage.h"
#include "GraphImage.h"
#include "DirectedGraphImage.h"
#include "UndirectedGraphImage.h"
#include "EdgeContext.h"
#include "Config.h"

const float GraphView::MIN_SCALE = 0.0625f;
const float GraphView::MAX_SCALE = 16.0f;
const float GraphView::SCALE_FACTOR = 1.25f;

GraphView::GraphView(Order order, Weight weighted) : _weighted(weighted)
{
	init(order, weighted);
}


GraphView::GraphView(QWidget * widget) : QGraphicsView(widget)
{
	init(Order::Undirected, Weight::Unwieghted);
}

GraphView::~GraphView()
{
	for (LabelMap::iterator it = _labelMap.begin(); it != _labelMap.end(); ++it)
	{
		delete (*it).second;
	}
	delete _graphScene;
	delete _graph;
}

void GraphView::init(Order order, Weight weighted)
{
	_scale = 1.0f;
	setFrameShape(QFrame::WinPanel);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setAlignment(Qt::AlignCenter);

	_graphScene = new QGraphicsScene;
	_graphScene->setSceneRect(QRect(-2000, -2000, 4000, 4000));
	setScene(_graphScene);

	_mouseClicked = false;
	_grabFlag = _addEdgeFlag = _rubberFlag = false;
	_rubberBand = nullptr;
	_edgeFlag = EdgeFlag::None;
	setMouseTracking(true);
	viewport()->setMouseTracking(true);

	_labelMap["source"] = new TextItem("Source");
	_labelMap["target"] = new TextItem("Target");
	scene()->addItem(_labelMap["source"]);
	scene()->addItem(_labelMap["target"]);

	bool isWeighted = _weighted == Weight::Weighted;

	GraphConfig * config = new GraphConfig(
		Application::Config::Instance().DefaultVertexContext()->clone(),
		Application::Config::Instance().DefaultEdgeContext()->clone(),
		Application::Config::Instance().SelectedVertexContext()->clone(),
		Application::Config::Instance().SelectedEdgeContext()->clone());

	switch (order)
	{
	case Order::Directed:
		_graph = new DirectedGraphImage(config, scene());
		break;
	default: case Order::Undirected:
		_graph = new UndirectedGraphImage(config, scene());
		break;
	}
	_graph->Weighted(_weighted == Weight::Weighted);

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

void GraphView::wheelEvent(QWheelEvent * event)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	// Zoom in
	if (event->delta() > 0 && _scale <= MAX_SCALE) {
		scale(SCALE_FACTOR, SCALE_FACTOR);
		_scale *= SCALE_FACTOR;
		emit scaleChanged(_scale);
	}
	// Zoom out
	else if (event->delta() < 0 && _scale >= MIN_SCALE) {
		float factor = 1.0f / SCALE_FACTOR;
		scale(factor, factor);
		_scale *= factor;
		emit scaleChanged(_scale);
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

void GraphView::mousePressEvent(QMouseEvent * event)
{
	auto chosenItems = items(event->pos());
	clickElement(event->pos(), chosenItems);
	QGraphicsView::mousePressEvent(event);
	viewport()->update();
}

void GraphView::mouseMoveEvent(QMouseEvent * event)
{
	if (_addEdgeFlag)
	{
		changeVerticesLabels(event->pos());
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

void GraphView::changeVerticesLabels(QPoint const & position)
{
	QPointF point(mapToScene(position));
	auto item = scene()->itemAt(point, transform());
	VertexImage * img = dynamic_cast<VertexImage*>(item);
	if (NULL == img)
		return;
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
		setEdgeFlag(EdgeFlag::Source);
		break;
	}
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

void GraphView::buildEdge(QGraphicsItem * const item)
{
	static std::pair<int, int> pair;
	static std::pair<QPointF, QPointF> coord;
	static bool firstVertexChecked = true;
	VertexImage * img = dynamic_cast<VertexImage*>(item);
	if (img == NULL)
		return;
	// jeœli to pierwszy wierzcho³ek
	if (firstVertexChecked)
	{
		pair.first = img->getVertex()->Id();
		coord.first = img->pos();
		setEdgeFlag(EdgeFlag::Target);
		firstVertexChecked = false;
	}
	else
	{
		pair.second = img->getVertex()->Id();
		coord.second = img->pos();
		setEdgeFlag(EdgeFlag::None);
		firstVertexChecked = true;
		_graph->addEdge(pair.first, pair.second, coord.first, coord.second);
		emit graphChanged();
	}
}

void GraphView::clickElement(QPoint const & position, QList<QGraphicsItem*> const & items)
{
	switch (Application::Config::Instance().CurrentTool())
	{
	case Tool::Vertex:
		if (items.size() == 0)
		{
			_graph->addVertex(mapToScene(position));
			emit graphChanged();
		}
		return;
	case Tool::Edge:
		if (items.size() == 0)
			return;
		buildEdge(items.first());
		return;
	case Tool::Pointer:
		pointItem(position, items);
		return;
	case Tool::Grab:
		grabItem(position);
		return;
	case Tool::RubberBand:
		startRubberBand(position);
		break;
	case Tool::Remove:
		if (items.size() != 0)
		{
			if (!items.first()->isSelected())
				_graph->removeItem(items);
			else
				_graph->removeItem(scene()->selectedItems());
			emit graphChanged();
		}
		return;
	default: case Tool::None:
		break;
	}
}
