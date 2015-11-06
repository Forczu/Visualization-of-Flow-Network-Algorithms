#pragma once

template<class T>
class ICloneable
{
protected:
	virtual T * clone() const = 0;
};