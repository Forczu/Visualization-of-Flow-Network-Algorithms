#pragma once
#include "TextItem.h"

class EdgeImage;

class EdgeTextItem : public TextItem
{
protected:
	EdgeImage * _subjectEdge;
public:
	explicit EdgeTextItem(EdgeImage * subject, QPointF const & position);

	void updateText();
	void turnOffEditorMode() override;
};

