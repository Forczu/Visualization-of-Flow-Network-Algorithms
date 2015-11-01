#include "EdgeTextItem.h"
#include "EdgeImage.h"
#include "Edge.h"

EdgeTextItem::EdgeTextItem(EdgeImage * subject, QPointF const & position)
: TextItem(position), _subjectEdge(subject)
{
	setParentItem(_subjectEdge);
	replaceFont(QFont("Calibri", 30, 0, false));
	setRegex("[[:digit:][:blank:]\\/[:blank:][:digit:]]+");	// "liczba / liczba"
	setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void EdgeTextItem::updateText()
{
	Edge * edge = _subjectEdge->getEdge();
	int flow = edge->getFlow();
	_text = (flow != 0 ? QString::number(flow) + " / " : "") + QString::number(edge->getCapacity());
}

void EdgeTextItem::turnOffEditorMode()
{
	TextItem::turnOffEditorMode();
	QStringList list = _text.split(QRegExp("\\D+"), QString::SkipEmptyParts);
	int flow, capacity;
	if (list.size() == 2)
	{
		flow = list[0].toInt();
		capacity = list[1].toInt();
		_subjectEdge->setFlow(flow);
	}
	else if (list.size() == 1)
	{
		capacity = list[0].toInt();
	}
	_subjectEdge->setCapacity(capacity);
}
