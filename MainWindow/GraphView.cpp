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
#include "GraphScene.h"
#include "Tool.h"

const float GraphView::MIN_SCALE = 0.0625f;
const float GraphView::MAX_SCALE = 16.0f;
const float GraphView::SCALE_FACTOR = 1.25f;

GraphView::GraphView(GraphImage * graph) : _graph(graph)
{
	createScene();
	init();
}

GraphView::GraphView(Order order, Weight weighted)
{
	createGraph(order, weighted);
	createScene();
	init();
}


GraphView::GraphView(QWidget * widget) : QGraphicsView(widget)
{
	createGraph(Order::Undirected, Weight::Weighted);
	createScene();
	init();
}

GraphView::~GraphView()
{
	delete _sourceLabel;
	delete _targetLabel;
	delete _graph;
}

void GraphView::init()
{
	_scale = 1.0f;
	setFrameShape(QFrame::WinPanel);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setAlignment(Qt::AlignCenter);

	_mouseClicked = false;
	_grabFlag = _rubberFlag = false;
	_rubberBand = nullptr;
	_edgeFlag = EdgeFlag::Source;
	setMouseTracking(true);
	viewport()->setMouseTracking(true);

	_sourceLabel = new TextItem("Source");
	_targetLabel = new TextItem("Target");
	scene()->addItem(_sourceLabel);
	scene()->addItem(_targetLabel);

	QFont font;
	font.setBold(true);
	font.setItalic(true);
	font.setPointSize(16);
	font.setFamily(QString("Calibri"));
	_sourceLabel->replaceFont(font);
	_targetLabel->replaceFont(font);
	_sourceLabel->setAlignment(Qt::AlignLeft);
	_targetLabel->setAlignment(Qt::AlignRight);
	_sourceLabel->hide();
	_targetLabel->hide();
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

void GraphView::createScene()
{
	setScene(GraphScene::getInstance());
	scene()->addItem(_graph);
}

void GraphView::createGraph(Order order, Weight weighted)
{
	GraphConfig * config = new GraphConfig(
		Application::Config::Instance().DefaultVertexContext()->clone(),
		Application::Config::Instance().DefaultEdgeContext()->clone(),
		Application::Config::Instance().SelectedVertexContext()->clone(),
		Application::Config::Instance().SelectedEdgeContext()->clone());
	switch (order)
	{
	case Order::Directed:
		_graph = new DirectedGraphImage(config);
		break;
	default: case Order::Undirected:
		_graph = new UndirectedGraphImage(config);
		break;
	}
	_graph->Weighted(weighted == Weight::Weighted);
}

void GraphView::glueLabel(EdgeFlag edgeFlag, VertexImage * img)
{
	switch (edgeFlag)
	{
	case EdgeFlag::Source:
		setSourceLabelPost(img);
		_sourceGlued = true;
		break;
	case EdgeFlag::Target:
		setTargetLabelPos(img);
		_targetGlued = true;
		break;
	}
}

void GraphView::setSourceLabelPost(VertexImage * img)
{
	_sourceLabel->setPos(
		img->scenePos().x() - img->Context()->Size() - _sourceLabel->boundingRect().width() / 2.0f,
		img->scenePos().y() - img->Context()->Size() - 40);
}

void GraphView::setTargetLabelPos(VertexImage * img)
{
	_targetLabel->setPos(
		img->scenePos().x() + img->Context()->Size() - _targetLabel->boundingRect().width() / 2.0f,
		img->scenePos().y() - img->Context()->Size() - 40);
}

void GraphView::unglueLabels()
{
	_sourceGlued = _targetGlued = false;
	_sourceLabel->hide();
	_targetLabel->hide();
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

void GraphView::grabItem(QPointF const & pos)
{
	setCursor(Qt::ClosedHandCursor);
	_offset = mapToScene(pos.toPoint()).toPoint();
	_grabFlag = true;
}

void GraphView::pointItem(QPointF const & position, QList<QGraphicsItem*> const & item)
{
	if (item.size() == 0)
	{
		startRubberBand(position);
	}
	else if (item.first()->isSelected())
	{
		grabItem(position);
	}
	else
	{
		unselectAll();
		for each (QGraphicsItem* item in item)
		{
			item->setSelected(true);
		}
	}
	viewport()->update();
}

void GraphView::startRubberBand(QPointF const & position)
{
	unselectAll();
	_origin = position.toPoint();
	if (_rubberBand == nullptr)
		_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
	_rubberBand->setGeometry(QRect(_origin, QSize()));
	_rubberBand->show();
	_rubberFlag = true;
}

void GraphView::mousePressEvent(QMouseEvent * event)
{
	QPoint position = event->pos();
	auto chosenItems = items(position);
	DrawingTool * tool = Application::Config::Instance().CurrentTool();
	tool->mousePressed(this, position, chosenItems);
	QGraphicsView::mousePressEvent(event);
	viewport()->update();
}

void GraphView::mouseMoveEvent(QMouseEvent * event)
{
	QPoint position = event->pos();
	DrawingTool * tool = Application::Config::Instance().CurrentTool();
	tool->mouseMoved(this, position);

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
	{
		if (_edgeFlag == EdgeFlag::Source && _sourceLabel->isVisible())
			_sourceLabel->hide();
		else if (_edgeFlag == EdgeFlag::Target && _targetLabel->isVisible())
			_targetLabel->hide();
		return;
	}
	switch (_edgeFlag)
	{
	case EdgeFlag::Source:
		if (!_sourceLabel->isVisible())
			_sourceLabel->show();
		setSourceLabelPost(img);
		break;
	case EdgeFlag::Target:
		if (!_targetLabel->isVisible())
			_targetLabel->show();
		setTargetLabelPos(img);
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

void GraphView::buildVertex(QPointF const & position, QList<QGraphicsItem*> const & items)
{
	if (items.size() != 0)
		return;
	_graph->addVertex(mapToScene(position.toPoint()));
	emit graphChanged();
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
		glueLabel(_edgeFlag, img);
		pair.first = img->getVertex()->Id();
		coord.first = img->pos();
		setEdgeFlag(EdgeFlag::Target);
		firstVertexChecked = false;
	}
	else
	{
		glueLabel(_edgeFlag, img);
		pair.second = img->getVertex()->Id();
		coord.second = img->pos();
		setEdgeFlag(EdgeFlag::Source);
		firstVertexChecked = true;
		_graph->addEdgeByDialog(pair.first, pair.second);
		unglueLabels();
		emit graphChanged();
	}
}

void GraphView::remove(QList<QGraphicsItem*> const &items)
{
	if (items.size() != 0)
	{
		if (!items.first()->isSelected())
			_graph->removeItem(items);
		else
			_graph->removeItem(scene()->selectedItems());
		emit graphChanged();
	}
}
