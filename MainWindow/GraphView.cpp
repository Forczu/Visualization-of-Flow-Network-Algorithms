#include "GraphView.h"
#include "VertexImage.h"
#include "TextItem.h"
#include "GraphImage.h"
#include "Config.h"
#include "Tool.h"
#include "Strings.h"

const float GraphView::MIN_SCALE = 0.0625f;
const float GraphView::MAX_SCALE = 16.0f;
const float GraphView::SCALE_FACTOR = 1.25f;

GraphView::GraphView(GraphImage * graph) : _graph(graph)
{
	init();
}

GraphView::GraphView(QWidget * parent /*= 0*/) : QGraphicsView(parent), _graph(nullptr)
{
	init();
}

GraphView::~GraphView()
{
	if (scene())
	{
		delete scene();
		setScene(nullptr);
	}
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
	_blocked = false;
	setMouseTracking(true);
	viewport()->setMouseTracking(true);

	createFont();
	createLabel(_sourceLabel, Strings::Instance().get(SOURCE), Qt::AlignLeft);
	createLabel(_targetLabel, Strings::Instance().get(TARGET), Qt::AlignRight);
}

void GraphView::createFont()
{
	_labelFont.setBold(true);
	_labelFont.setItalic(true);
	_labelFont.setPointSize(16);
	_labelFont.setFamily(QString("Calibri"));
}

void GraphView::createLabel(QPointer<TextItem> & label, QString const & text, Qt::AlignmentFlag align) const
{
	label = new TextItem(text);
	label->replaceFont(_labelFont);
	label->setAlignment(align);
	label->hide();
}

void GraphView::unselectAll(QGraphicsItem * const except) const
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

void GraphView::setSourceLabelPost(VertexImage * img) const
{
	_sourceLabel->setPos(
		img->scenePos().x() - img->Context()->Size() - _sourceLabel->boundingRect().width() / 2.0f,
		img->scenePos().y() - img->Context()->Size() - _sourceLabel->boundingRect().height());
}

void GraphView::setTargetLabelPos(VertexImage * img) const
{
	_targetLabel->setPos(
		img->scenePos().x() + img->Context()->Size() - _targetLabel->boundingRect().width() / 2.0f,
		img->scenePos().y() - img->Context()->Size() - _targetLabel->boundingRect().height());
}

void GraphView::unglueLabels()
{
	_sourceGlued = _targetGlued = false;
	_sourceLabel->hide();
	_targetLabel->hide();
}

/// <summary>
/// Funkcja zwraca listê elementów grafu w wybranym punkcie, ale bez obiektu pojemnika (samego grafu).
/// </summary>
/// <param name="position">The position.</param>
/// <returns>Lista elementów grafu we wskazanym punkcie.</returns>
QList<QGraphicsItem*> GraphView::takeGraphElements(QPoint const & position) const
{
	auto chosenItems = items(position);
	for (QList<QGraphicsItem*>::iterator it = chosenItems.begin(); it != chosenItems.end(); )
	{
		if (dynamic_cast<GraphImage*>(*it) != NULL)
			it = chosenItems.erase(it);
		else
			++it;
	}
	return chosenItems;
}

void GraphView::zoom(QPointF const & pos, Qt::KeyboardModifiers const & modifiers)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	if (modifiers & Qt::AltModifier)
	{
		zoomOut();
	}
	else
	{
		zoomIn();
	}
}

void GraphView::wheelEvent(QWheelEvent * event)
{
	if (_blocked)
		return;
	event->delta() > 0 ? zoomIn() : zoomOut();
}

void GraphView::zoomOut()
{
	float widthFactor = viewport()->width() / scene()->width();
	float heightFactor = viewport()->height() / scene()->height();
	if (_scale >= widthFactor && _scale >= heightFactor)
	{
		float factor = 1.0f / SCALE_FACTOR;
		scale(factor, factor);
		_scale *= factor;
		emit scaleChanged(_scale);
		_graph->updateScale(_scale);
	}
}

void GraphView::zoomIn()
{
	if (_scale <= MAX_SCALE)
	{
		scale(SCALE_FACTOR, SCALE_FACTOR);
		_scale *= SCALE_FACTOR;
		emit scaleChanged(_scale);
		_graph->updateScale(_scale);
	}
}

void GraphView::grabItem(QPointF const & pos)
{
	setCursor(Qt::ClosedHandCursor);
	_offset = mapToScene(pos.toPoint()).toPoint();
	_grabFlag = true;
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

void GraphView::setGraphImage(GraphImage * val, QPointF const & position /*= QPointF()*/)
{
	if (!val)
		return;
	if (scene() && val != _graph.data())
	{
		_graph = val;
		scene()->addItem(_graph);
		_graph->setPos(position);
	}
}

void GraphView::mousePressEvent(QMouseEvent * event)
{
	if (_blocked)
		return;
	QPoint position = event->pos();
	QList<QGraphicsItem*> chosenItems = takeGraphElements(position);
	DrawingTool * tool = Application::Config::Instance().CurrentTool();
	tool->mousePressed(this, position, chosenItems, event->modifiers());
	QGraphicsView::mousePressEvent(event);
	updateView();
}

void GraphView::mouseMoveEvent(QMouseEvent * event)
{
	if (_blocked)
		return;
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

void GraphView::mouseReleaseEvent(QMouseEvent * event)
{
	if (_blocked)
		return;
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

void GraphView::keyPressEvent(QKeyEvent *event)
{
	if (event->modifiers() & Qt::ShiftModifier)
	{
		QList<QGraphicsItem*> chosenItems = scene()->selectedItems();
		alignItems(chosenItems, event->key());
	}
	QGraphicsView::keyPressEvent(event);
}

void GraphView::alignItems(QList<QGraphicsItem*> const & items, int key)
{
	using Qt::Key;
	QList<QGraphicsItem*>::const_iterator it;
	switch (key)
	{
	case Key::Key_Up:
		it = std::min_element(items.begin(), items.end(), [](QGraphicsItem * a, QGraphicsItem * b) -> bool
		{
			return a->pos().y() < b->pos().y();
		});
		break;
	case Key::Key_Down:
		it = std::max_element(items.begin(), items.end(), [](QGraphicsItem * a, QGraphicsItem * b) -> bool
		{
			return a->pos().y() < b->pos().y();
		});
	case Key::Key_Right:
		it = std::max_element(items.begin(), items.end(), [](QGraphicsItem * a, QGraphicsItem * b) -> bool
		{
			return a->pos().x() < b->pos().x();
		});
		break;
	case Key::Key_Left:
		it = std::min_element(items.begin(), items.end(), [](QGraphicsItem * a, QGraphicsItem * b) -> bool
		{
			return a->pos().x() < b->pos().x();
		});
		break;
	}
	switch (key)
	{
	case Key::Key_Up: case Key::Key_Down:
		for (QGraphicsItem * item : items)
		{
			float x = item->pos().x(), y = (*it)->pos().y();
			item->setPos(x, y);
		}
		break;
	case Key::Key_Right: case Key::Key_Left:
		for (QGraphicsItem * item : items)
		{
			float x = (*it)->pos().x(), y = item->pos().y();
			item->setPos(x, y);
		}
		break;
	}
	_graph->updateEdges();
	updateView();
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

void GraphView::addScene(QGraphicsScene * scene)
{
	setScene(scene);
	if (_graph && _graph != nullptr)
		scene->addItem(_graph);
	scene->addItem(_sourceLabel);
	scene->addItem(_targetLabel);
}

void GraphView::updateView()
{
	update();
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

void GraphView::buildVertex(QPointF const & position, QList<QGraphicsItem*> const & items)
{
	// je¿eli kliknêliœmy w powierzchniê grafu, ale nie zajmowan¹ przez
	// ¿aden z jego elementów, to dodajemy nowy wierzcho³ek
	if (items.size() != 0 && dynamic_cast<GraphImage*>(items.first()) == NULL)
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
		unglueLabels();
		if (_graph->edgeExists(pair.first, pair.second))
			return;
		_graph->addEdgeByDialog(pair.first, pair.second, _scale);
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
