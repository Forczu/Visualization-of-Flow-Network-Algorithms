#include "VertexContext.h"
#include "VertexBuilder.h"

VertexContext::VertexContext(VertexContext const & other)
{
	Size(other.Size());
	StrokeSize(other.StrokeSize());
	Color(other.Color());
	StrokeColor(other.StrokeColor());
	Font(other.Font());
	Style(other.Style());
}

VertexContext::VertexContext(VertexBuilder * builder)
{
	_size = builder->_size;
	_strokeSize = builder->_strokeSize;
	_color = builder->_color;
	_strokeColor = builder->_strokeColor;
	_font = builder->_font;
}
