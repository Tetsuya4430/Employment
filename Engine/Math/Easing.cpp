#include "Easing.h"

float Easing::EaseIn(float Move, float BasePos, float MoveTime, float Time)
{
	Time /= MoveTime;

	return Move * Time * Time + BasePos;
}
