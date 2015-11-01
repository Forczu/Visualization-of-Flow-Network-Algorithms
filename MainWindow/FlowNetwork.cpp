#include "FlowNetwork.h"
#include "QGraphicsSceneEvent"
#include "VertexImage.h"
#include "TextItem.h"


FlowNetwork::FlowNetwork(GraphConfig * config)
: DirectedGraphImage(config), _source(0), _target(0)
{
	setAcceptedMouseButtons(Qt::MouseButton::RightButton);
	init();
}


FlowNetwork::FlowNetwork(FlowNetwork const & network)
: DirectedGraphImage(network)
{
	_source = network._source;
	_target = network._target;
	_labelFont = network._labelFont;
	_sourceLabel = new TextItem(*network._sourceLabel);
	_targetLabel = new TextItem(*network._targetLabel);
}

FlowNetwork::~FlowNetwork()
{
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

void FlowNetwork::createLabel(TextItem *& label, QString const & text, Qt::AlignmentFlag align)
{
	label = new TextItem(text);
	label->replaceFont(_labelFont);
	label->setAlignment(align);
	label->hide();
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
				QString source = "Oznacz jako Ÿród³o", target = "Oznacz jako ujœcie";
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

void FlowNetwork::markSource(int id, VertexImage * vertex)
{
	_source = id;
	_sourceLabel->setPos(
		- vertex->Context()->Size() - _sourceLabel->boundingRect().width() / 2.0f,
		- vertex->Context()->Size() - _sourceLabel->boundingRect().height());
	_sourceLabel->setParentItem(vertex);
	if (!_sourceLabel->isVisible()) 
		_sourceLabel->show();
}

void FlowNetwork::markTarget(int id, VertexImage * vertex)
{
	_target = id;
	_targetLabel->setPos(
		+ vertex->Context()->Size() - _targetLabel->boundingRect().width() / 2.0f,
		- vertex->Context()->Size() - _targetLabel->boundingRect().height());
	_targetLabel->setParentItem(vertex);
	if (!_targetLabel->isVisible())
		_targetLabel->show();
}

FlowNetwork * FlowNetwork::makeResidualNetwork()
{
	FlowNetwork * residualNetwork = new FlowNetwork(*this);
	return residualNetwork;
}
