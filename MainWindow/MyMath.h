#pragma once
#include <cmath>
class MyMath
{
public:
	static const float HALF_FULL_ANGLE;
	static const float FULL_ANGLE;
	static const double PI;

	static const double toRadian(double deg)
	{
		return deg * pi() / HALF_FULL_ANGLE;
	}
	static const float toRadian(float deg)
	{
		return deg * pi() / HALF_FULL_ANGLE;
	}
	static const double pi()
	{
		return PI;
	}
	static const float wrapAngle(float angle)
	{
		angle = std::fmod(angle, FULL_ANGLE);
		if (angle < 0.0f)
			angle += FULL_ANGLE;
		return angle;
	}
};