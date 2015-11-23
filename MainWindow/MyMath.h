#pragma once
class MyMath
{
public:
	static const double toRadian(double deg)
	{
		return deg * pi() / 180.0;
	}
	static const float toRadian(float deg)
	{
		return deg * pi() / 180.0f;
	}
	static const double pi()
	{
		return 3.14159265358979323846;
	}
};