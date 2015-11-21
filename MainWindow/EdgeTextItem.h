#pragma once
#include "TextItem.h"

class EdgeImage;

class EdgeTextItem : public TextItem
{
protected:
	EdgeImage * _subjectEdge;
	QString _oldStr;
public:
	EdgeTextItem(EdgeImage * subject, QPointF const & position);

	void updateText();
	void turnOffEditorMode() override;
};

