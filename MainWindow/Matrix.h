#pragma once

#include <vector>
#include <iostream>

class Matrix
{
	typedef std::vector<std::vector<short>> Tab2D;
	Tab2D _matrix;
	unsigned short _rows;
	unsigned short _cols;

public:
	Matrix(unsigned short rows, unsigned short cols, short initial);
	~Matrix();

	short operator()(unsigned short row, unsigned short col);
	void Set(unsigned short row, unsigned short col, unsigned short val);

	inline Tab2D const & Get() const
	{
		return _matrix;
	}

	friend std::ostream& operator<< (std::ostream &out, Matrix const & matrix);
};

