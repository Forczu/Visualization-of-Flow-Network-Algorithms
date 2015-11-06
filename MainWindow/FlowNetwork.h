#pragma once
#include "DirectedGraphImage.h"
#include "ICloneable.h"
#include <map>

class TextItem;

class FlowNetwork : public DirectedGraphImage, public ICloneable<FlowNetwork>
{
	int _source;
	int _target;
	TextItem * _sourceLabel;
	TextItem * _targetLabel;
	std::map<int, int> _flowMap;
	std::map<int, int> _capacityMap;
	QFont _labelFont;

public:
	FlowNetwork(GraphConfig * config);
	FlowNetwork(FlowNetwork const & network);
	~FlowNetwork();

	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

	void markSource(int id, VertexImage * vertex);
	void markTarget(int id, VertexImage * vertex);

	FlowNetwork * makeResidualNetwork();
	FlowNetwork * clone() const override
	{
		return new FlowNetwork(*this);
	}

private:
	void init();
	void createFont();
	void createLabel(TextItem *& label, QString const & text, Qt::AlignmentFlag align);
};

