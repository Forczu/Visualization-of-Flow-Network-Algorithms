#pragma once

template<class T>
class ICloneable
{
public:
	virtual T * clone() const = 0;
};