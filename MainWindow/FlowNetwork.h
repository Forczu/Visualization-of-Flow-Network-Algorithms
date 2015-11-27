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

	static GraphImage * getInstance(GraphConfig * config)
	{
		return new FlowNetwork(config);
	}

	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

	void markSource(int id);
	void markTarget(int id);
	void markSource(int id, VertexImage * vertex);
	void markTarget(int id, VertexImage * vertex);

	FlowNetwork * makeResidualNetwork();
	FlowNetwork * clone() const override
	{
		return new FlowNetwork(*this);
	}

	inline int getSource() const { return _source; }
	inline int getTarget() const { return _target; }
	void setSource(int source) { _source = source; }
	void setTarget(int target) { _target = target; }

	bool checkCapacityCondition();
	bool checkFlowPreservation();
	bool checkStructure();

private:
	void init();
	void createFont();
	void createLabel(TextItem *& label, QString const & text, Qt::AlignmentFlag align);
};

