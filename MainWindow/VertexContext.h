#pragma once

#include <QColor>
#include <QFont>

class VertexBuilder;

class VertexContext
{
	short _size;
	short _strokeSize;
	QColor _color;
	QColor _strokeColor;
	QFont _font;

public:
	VertexContext(VertexBuilder * builder);
	VertexContext(VertexContext const & other);
	
public:
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

	inline VertexContext * clone() const { return new VertexContext(*this); }
};
