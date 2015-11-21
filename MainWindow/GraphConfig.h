#pragma once
#include "VertexContext.h"
#include "EdgeContext.h"
#include <QString>

class GraphConfig
{
protected:
	QString _name;
	VertexContext * _normalVertexContext;
	EdgeContext * _normalEdgeContext;
	VertexContext * _selectedVertexContext;
	EdgeContext * _selectedEdgeContext;
public:
	GraphConfig(VertexContext * normalVertexContext, EdgeContext * normalEdgeContext,
		VertexContext * selectedVertexContext, EdgeContext * selectedEdgeContext);
	GraphConfig(GraphConfig const & other);
	~GraphConfig();

	inline VertexContext * NormalVertexContext() const { return _normalVertexContext; }
	void NormalVertexContext(VertexContext * val) { _normalVertexContext = val; }
	inline EdgeContext * NormalEdgeContext() const { return _normalEdgeContext; }
	void NormalEdgeContext(EdgeContext * val) { _normalEdgeContext = val; }
	inline VertexContext * SelectedVertexContext() const { return _selectedVertexContext; }
	void SelectedVertexContext(VertexContext * val) { _selectedVertexContext = val; }
	inline EdgeContext * SelectedEdgeContext() const { return _selectedEdgeContext; }
	void SelectedEdgeContext(EdgeContext * val) { _selectedEdgeContext = val; }
	inline QString getName() const { return _name; }
	void setName(QString const & val) { _name = val; }

	inline GraphConfig * clone() const { return new GraphConfig(*this); }
};

