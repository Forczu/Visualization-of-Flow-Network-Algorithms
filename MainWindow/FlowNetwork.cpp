#include "FlowNetwork.h"
#include "VertexImage.h"
#include "TextItem.h"
#include "EdgeImage.h"
#include "Strings.h"

FlowNetwork::FlowNetwork(GraphConfig * config)
: DirectedGraphImage(config), _source(0), _target(0)
{
	setAcceptedMouseButtons(Qt::MouseButton::RightButton);
	init();
}


FlowNetwork::FlowNetwork(FlowNetwork const & network)
: DirectedGraphImage(network)
{
	init();
	markSource(network._source);
	markTarget(network._target);
}

FlowNetwork::~FlowNetwork()
{
	delete _sourceLabel;
	delete _targetLabel;
}

void FlowNetwork::init()
{
	createFont();
	createLabel(_sourceLabel, "s", Qt::AlignLeft);
	createLabel(_targetLabel, "t", Qt::AlignRight);
}

void FlowNetwork::createFont()
{
	_labelFont.setBold(true);
	_labelFont.setItalic(true);
	_labelFont.setPointSize(48);
	_labelFont.setFamily(QString("Calibri"));
}

void FlowNetwork::createLabel(QPointer<TextItem>& label, QString const & text, Qt::AlignmentFlag align)
{
	label = new TextItem(text);
	label->replaceFont(_labelFont);
	label->setAlignment(align);
	label->hide();
}

void FlowNetwork::removeVertex(VertexImage * const vertex)
{
	int vertexId = vertex->getId();
	if (_vertexMap.contains(vertexId))
	{
		if (vertexId == _source)
			_source = 0;
		if (vertexId == _target)
			_target = 0;
	}
	GraphImage::removeVertex(vertex);
}

/// <summary>
/// Sprawdzenie warunku zachowania przepustowoœci.
/// ¯adna krawêdŸ nie mo¿e mieæ przep³ywu wiêkszego od przepustowoœci.
/// </summary>
/// <returns>True, je¿eli za³o¿enia s¹ spe³nione, false jeœli nie</returns>
bool FlowNetwork::checkCapacityCondition(CheckInfo &info)
{
	bool succeeded = true;
	for (EdgeImage * edge : _edgeMap)
	{
		if (edge->getFlow() <= edge->getCapacity())
			continue;
		info += Strings::Instance().get(FLOW_GREATER_THAN_CAPACITY)
			.arg(edge->VertexFrom()->getId()).arg(edge->VertexTo()->getId());
		succeeded = false;
	}
	return succeeded;
}


/// <summary>
/// Sprawdzenie warunku zachowania przep³ywu.
/// Suma przep³ywów wp³ywaj¹cych do wierzcho³ka, nie bêd¹cego Ÿród³em ani ujœciem, musi byæ równa sumie wyp³ywaj¹cych z niego.
/// </summary>
/// <returns>True, je¿eli za³o¿enia s¹ spe³nione, false jeœli nie</returns>
bool FlowNetwork::checkFlowPreservation(CheckInfo &info)
{
	bool succeeded = true;
	for (VertexImage * vertex : _vertexMap)
	{
		if (_source == vertex->getId() || _target == vertex->getId())
			continue;
		std::vector<EdgeImage*> inEdges, outEdges;
		std::for_each(_edgeMap.begin(), _edgeMap.end(), [&](EdgeImage * edge)
		{
			if (edge->VertexTo() == vertex)
				inEdges.push_back(edge);
			if (edge->VertexFrom() == vertex)
				outEdges.push_back(edge);
		});
		int inSum = 0, outSum = 0;
		std::for_each(inEdges.begin(), inEdges.end(), [&](EdgeImage * edge)
		{
			inSum += edge->getFlow();
		});
		std::for_each(outEdges.begin(), outEdges.end(), [&](EdgeImage * edge)
		{
			outSum += edge->getFlow();
		});
		if (inSum != outSum)
		{
			info += Strings::Instance().get(SUMS_OF_INFLOWS_AND_OUTFLOWS_NOT_EQUAL)
				.arg(vertex->getId());
			succeeded = false;
		}
	}
	return succeeded;
}

/// <summary>
/// Sprawdzenie czy budowa sieci spe³nia za³o¿enie, ¿e z ka¿dego wierzcho³ka, nie bêd¹cego Ÿród³em ani ujœciem,
/// istnieje œcie¿ka prowadz¹ca do Ÿród³a.
/// </summary>
/// <returns>True, je¿eli struktura jest w porz¹dku, jeœli nie to false</returns>
bool FlowNetwork::checkStructure(CheckInfo &info)
{
	VertexImage * source, * target;
	bool succeeded = true;
	for (VertexImage * vertex : _vertexMap)
	{
		if (_source == vertex->getId() || _target == vertex->getId())
			continue;
		if (_vertexMap.find(getSourceId()) != _vertexMap.end())
			source = _vertexMap[getSourceId()];
		else
			source = NULL;
		if (_vertexMap.find(getTargetId()) != _vertexMap.end())
			target = _vertexMap[getTargetId()];
		else
			target = NULL;
		auto it = std::find_if(_edgeMap.begin(), _edgeMap.end(), [&](EdgeImage * edge)
		{
			if (edge->VertexFrom() == vertex && edge->VertexTo() != source)
				return true;
			return false;
		});
		if (it == _edgeMap.end())
		{
			info += Strings::Instance().get(NO_ROUTE_TO_TARGET).arg(vertex->getId());
			succeeded = false;
		}
		it = std::find_if(_edgeMap.begin(), _edgeMap.end(), [&](EdgeImage * edge)
		{
			if (edge->VertexTo() == vertex && edge->VertexFrom() != target)
				return true;
			return false;
		});
		if (it == _edgeMap.end())
		{
			info += Strings::Instance().get(NO_ROUTE_FROM_SOURCE).arg(vertex->getId());
			succeeded = false;
		}
	}
	return succeeded;
}

void FlowNetwork::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	auto items = childItems();
	for (QGraphicsItem * item : items)
	{
		QRectF itemRect = item->mapRectToScene(item->boundingRect());
		if (itemRect.contains(event->scenePos()))
		{
			VertexImage * vImg = dynamic_cast<VertexImage*>(item);
			if (NULL != vImg)
			{
				QString source = Strings::Instance().get(MARK_SOURCE), target = Strings::Instance().get(MARK_TARGET);
				QMenu menu;
				QAction * setAsSource = menu.addAction(source);
				QAction * setAsTarget = menu.addAction(target);
				QAction * selectedAction = menu.exec(event->screenPos());
				if (selectedAction == setAsSource)
					markSource(vImg->getVertex()->Id(), vImg);
				else if (selectedAction == setAsTarget)
					markTarget(vImg->getVertex()->Id(), vImg);
				delete setAsSource;
				delete setAsTarget;
			}
		}
	}
}

void FlowNetwork::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	drawLabel(_sourceLabel, _source, painter);
	drawLabel(_targetLabel, _target, painter);
	DirectedGraphImage::paint(painter, option, widget);
}

void FlowNetwork::drawLabel(QPointer<TextItem>& label, int key, QPainter * painter)
{
	if (label && label->isVisible() && label->isSelected())
	{
		QLineF connection = QLineF(label->scenePos(), _vertexMap[key]->pos());
		painter->setPen(QPen(Qt::black, 2, Qt::DotLine));
		painter->drawLine(connection);
	}
}

/// <summary>
/// Oznacza wybrany wierzcho³ek jako Ÿród³o.
/// </summary>
/// <param name="id">The identifier.</param>
/// <param name="vertex">The vertex.</param>
void FlowNetwork::markSource(int id, VertexImage * vertex)
{
	if (!_sourceLabel)
		createLabel(_sourceLabel, "s", Qt::AlignLeft);
	_source = id;
	_sourceLabel->setPos(
		- vertex->Context()->Size() - _sourceLabel->boundingRect().width() / 2.0f,
		- vertex->Context()->Size() - _sourceLabel->boundingRect().height());
	_sourceLabel->setParentItem(vertex);
	if (!_sourceLabel->isVisible()) 
		_sourceLabel->show();
}

void FlowNetwork::markSource(int id)
{
	if (id == 0)
		return;
	VertexImage * vertex = _vertexMap[id];
	markSource(id, vertex);
}

void FlowNetwork::markTarget(int id, VertexImage * vertex)
{
	if (!_targetLabel)
		createLabel(_targetLabel, "t", Qt::AlignRight);
	_target = id;
	_targetLabel->setPos(
		+ vertex->Context()->Size() - _targetLabel->boundingRect().width() / 2.0f,
		- vertex->Context()->Size() - _targetLabel->boundingRect().height());
	_targetLabel->setParentItem(vertex);
	if (!_targetLabel->isVisible())
		_targetLabel->show();
}

void FlowNetwork::markTarget(int id)
{
	if (id == 0)
		return;
	VertexImage * vertex = _vertexMap[id];
	markTarget(id, vertex);
}
