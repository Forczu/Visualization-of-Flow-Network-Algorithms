#include "EdgeTextItem.h"
#include "EdgeImage.h"
#include "Edge.h"
#include <QMessageBox>

EdgeTextItem::EdgeTextItem(EdgeImage * subject, QPointF const & position)
: TextItem(position, subject), _subjectEdge(subject)
{
	replaceFont(QFont("Calibri", 30, 0, false));
	setRegex("[[:digit:][:blank:]\\/[:blank:][:digit:]]+");	// "liczba / liczba"
	setFlag(QGraphicsItem::ItemIgnoresTransformations);
	_oldStr = _text;
	setPos(_subjectEdge->center());
}

void EdgeTextItem::updateText()
{
	Edge * edge = _subjectEdge->getEdge();
	int flow = edge->getFlow();
	_oldStr = _text = (flow != 0 ? QString::number(flow) + " / " : "") + QString::number(edge->getCapacity());
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
		if (flow <= capacity)
		{
			_subjectEdge->setValues(flow, capacity);
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle("B³êdna wartoœæ.");
			msgBox.setText("Przep³yw nie mo¿e byæ wiêkszy ni¿ przepusowoœæ.");
			msgBox.exec();
			_text = _oldStr;
			_textEdit->setPlainText(_text);
		}
	}
	else if (list.size() == 1)
	{
		capacity = list[0].toInt();
		_subjectEdge->setCapacity(capacity);
	}
	_oldStr = _text;
}
