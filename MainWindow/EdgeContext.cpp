#include "EdgeContext.h"


EdgeContext::EdgeContext() : _size(1), _color(QColor())
{
}


EdgeContext::EdgeContext(int size, QColor const & color) : _size(size), _color(color)
{
}
