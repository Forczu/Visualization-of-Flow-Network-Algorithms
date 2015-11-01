#pragma once
#include "DirectedGraphImage.h"
#include <map>

class TextItem;

class FlowNetwork :	public DirectedGraphImage
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
	~FlowNetwork();

	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

	void markSource(int id, VertexImage * vertex);
	void markTarget(int id, VertexImage * vertex);

	FlowNetwork * makeResidualNetwork();

private:
	void init();
	void createFont();
	void createLabel(TextItem *& label, QString const & text, Qt::AlignmentFlag align);
};

