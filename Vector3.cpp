#include "Vector3.h"

using namespace DirectX;

XMFLOAT3 Vector3::lerp(XMFLOAT3 position, const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	/*XMFLOAT3 ReturnPos;

	ReturnPos.x = start.x * (1.0f - t) + end.x * t;
	ReturnPos.y = start.y * (1.0f - t) + end.y * t;
	ReturnPos.z = start.z * (1.0f - t) + end.z * t;

	return ReturnPos;*/
	XMFLOAT3 Vec = {};
	Vec.x =  (end.x - position.x) / t;
	Vec.y = (end.y - position.y) / t;
	Vec.z = (end.z - position.z) / t;

	position.x += Vec.x;
	position.z += Vec.y;
	position.z += Vec.y;

	return position;

}

const XMFLOAT3 Vector3::easeIn(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	return XMFLOAT3();
}

const XMFLOAT3 Vector3::easeOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	return XMFLOAT3();
}

const XMFLOAT3 Vector3::easeInOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	return XMFLOAT3();
}
