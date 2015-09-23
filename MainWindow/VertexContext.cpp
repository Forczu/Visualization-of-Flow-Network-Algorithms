#include "VertexContext.h"

VertexContext::VertexContext(short size, short strokeSize, QColor const & color, QColor const & strokeColor, QFont const & font)
: _size(size), _strokeSize(strokeSize), _color(color), _strokeColor(strokeColor), _font(font)
{
}

VertexContext::VertexContext() : _size(1), _strokeSize(1)
{

}
