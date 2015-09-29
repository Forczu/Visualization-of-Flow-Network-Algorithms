#pragma once

#include <QColor>

class EdgeContext
{
	int _size;
	QColor _color;
public:
	EdgeContext();
	EdgeContext(int size, QColor const & color);

	inline int Size() const { return _size; }
	void Size(int val) { _size = val; }
	inline QColor Color() const { return _color; }
	void Color(QColor val) { _color = val; }
};

