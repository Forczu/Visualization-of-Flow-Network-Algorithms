#include "EdgeContext.h"


EdgeContext::EdgeContext() : _size(1), _color(QColor())
{
}


EdgeContext::EdgeContext(int size, QColor const & color) : _size(size), _color(color)
{
}

EdgeContext::EdgeContext(EdgeContext const & other)
{
	Size(other.Size());
	Color(other.Color());
}
