#pragma once
#include "VertexContext.h"
#include "EdgeContext.h"

class GraphConfig
{
protected:
	VertexContext * _normalVertexContext;
	EdgeContext * _normalEdgeContext;
	VertexContext * _selectedVertexContext;
	EdgeContext * _selectedEdgeContext;
	bool _isWeighted;

public:
	GraphConfig(VertexContext * normalVertexContext, EdgeContext * normalEdgeContext,
		VertexContext * selectedVertexContext, EdgeContext * selectedEdgeContext,
		bool weighted);
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

	inline GraphConfig * clone() const { return new GraphConfig(*this); }
};

