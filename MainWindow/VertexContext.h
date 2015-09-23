#pragma once

#include <QColor>
#include <QFont>

class VertexContext
{
	short _size;
	short _strokeSize;
	QColor _color;
	QColor _strokeColor;
	QFont _font;
public:
	VertexContext();
	VertexContext(short size, short strokeSize, QColor const & color, QColor const & strokeColor, QFont const & font);

	inline short Size() const { return _size; }
	void Size(short val) { _size = val; }
	inline short StrokeSize() const { return _strokeSize; }
	void StrokeSize(short val) { _strokeSize = val; }
	inline QColor const & Color() const { return _color; }
	void Color(QColor val) { _color = val; }
	inline QColor const & StrokeColor() const { return _strokeColor; }
	void StrokeColor(QColor val) { _strokeColor = val; }
	inline QFont const & Font() const { return _font; }
	void Font(QFont val) { _font = val; }
};
