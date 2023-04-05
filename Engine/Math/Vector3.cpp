#include "Vector3.h"

using namespace DirectX;

const double Vector3::easeIn(const float time, const float start, const float startToend, float end)
{
	double x = time / end;

	double vec;

	vec = ease_in_cubic(x);

	double ret;

	ret = startToend * vec + start;

	return ret;
}

const double Vector3::ease_in_cubic(double x)
{
	return x * x * x;
}
