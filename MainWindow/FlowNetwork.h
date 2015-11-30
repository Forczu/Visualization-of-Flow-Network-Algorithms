#pragma once
#include "DirectedGraphImage.h"
#include "ICloneable.h"
#include "CheckInfo.h"
#include "TextItem.h"
#include <QPointer>

class FlowNetwork : public DirectedGraphImage, public ICloneable<FlowNetwork>
{
	int _source;
	int _target;
	QPointer<TextItem> _sourceLabel;
	QPointer<TextItem> _targetLabel;
	QFont _labelFont;

public:
	FlowNetwork(GraphConfig * config);
	FlowNetwork(FlowNetwork const & network);
	~FlowNetwork();

	inline static GraphImage * getInstance(GraphConfig * config) { return new FlowNetwork(config); }
	inline FlowNetwork * clone() const override { return new FlowNetwork(*this); }

	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

	void markSource(int id);
	void markTarget(int id);
	void markSource(int id, VertexImage * vertex);
	void markTarget(int id, VertexImage * vertex);

	inline int getSource() const { return _source; }
	inline int getTarget() const { return _target; }
	void setSource(int source) { _source = source; }
	void setTarget(int target) { _target = target; }

	bool checkCapacityCondition(CheckInfo &info);
	bool checkFlowPreservation(CheckInfo &info);
	bool checkStructure(CheckInfo &info);

private:
	void init() override;
	void createFont();
	void createLabel(QPointer<TextItem>& label, QString const & text, Qt::AlignmentFlag align) const;
	void drawLabel(QPointer<TextItem>& label, int key, QPainter * painter);
};

