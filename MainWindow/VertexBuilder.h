#pragma once

#include "VertexContext.h"

class VertexBuilder
{
	friend VertexContext;
	short _size;
	short _strokeSize;
	QColor _color;
	QColor _strokeColor;
	QFont _font;
public:
	VertexBuilder(short size, short strokeSize)
	{
		_size = size;
		_strokeSize = strokeSize;
	}
	VertexBuilder * color(QColor const & color) { _color = color; return this; }
	VertexBuilder * strokeColor(QColor const & strokeColor) { _strokeColor = strokeColor; return this; }
	VertexBuilder * font(QFont const & font) { _font = font; return this; }
	VertexContext * build() { return new VertexContext(this); }
};