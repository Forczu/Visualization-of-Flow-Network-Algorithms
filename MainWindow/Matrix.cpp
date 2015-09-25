#include "Matrix.h"

Matrix::Matrix(unsigned short rows, unsigned short cols, short initial)
{
	_rows = rows;
	_cols = cols;
	for (int i = 0; i < rows; i++)
	{
		_matrix.push_back(std::vector<short>(cols, initial));
	}
}

Matrix::~Matrix()
{
}

short Matrix::operator()(unsigned short row, unsigned short col)
{
	return _matrix[row][col];
}

void Matrix::Set(unsigned short row, unsigned short col, unsigned short val)
{
	_matrix[row][col] = val;
}

std::ostream& operator<< (std::ostream &out, Matrix const & matrix)
{
	for each (auto vector in matrix.Get())
	{
		for each (auto element in vector)
		{
			std::cout << element << ' ';
		}
		std::endl(std::cout);
	}
	return out;
}
