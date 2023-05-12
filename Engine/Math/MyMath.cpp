#include "MyMath.h"
using namespace DirectX;

const XMFLOAT3 MyMath::lerp(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
    XMFLOAT3 A, B;
    A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
    B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

    XMFLOAT3 ReturnPos = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);

    return ReturnPos;
}

const XMFLOAT2 MyMath::lerp_2(const XMFLOAT2& start, const XMFLOAT2& end, const float t)
{
    XMFLOAT2 A, B;
    A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
    B = XMFLOAT2(end.x * t, end.y * t);

    XMFLOAT2 ReturnPos = XMFLOAT2(A.x + B.x, A.y + B.y);

    return ReturnPos;
}