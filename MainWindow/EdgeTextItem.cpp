#include "EdgeTextItem.h"
#include "EdgeImage.h"
#include "Edge.h"

EdgeTextItem::EdgeTextItem(EdgeImage * subject, QPointF const & position)
: TextItem(position), _subjectEdge(subject)
{
	setParentItem(_subjectEdge);
	replaceFont(QFont("Calibri", 30, 0, false));
	setRegex("[[:digit:][:blank:]/[:blank:][:digit:]]+");	// "liczba / liczba"
	setFlag(QGraphicsItem::ItemIgnoresTransformations);
}


EdgeTextItem::~EdgeTextItem()
{
}

void EdgeTextItem::updateText()
{
	Edge * edge = _subjectEdge->getEdge();
	int flow = edge->getFlow();
	_text = (flow != 0 ? QString::number(flow) + " / " : "") + QString::number(edge->getCapacity());
}
