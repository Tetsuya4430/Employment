#pragma once
#include <DirectXMath.h>
class Vector3
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	//補間関数
	static XMFLOAT3 lerp(XMFLOAT3 position, const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	const XMFLOAT3 easeIn(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	const XMFLOAT3 easeOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	const XMFLOAT3 easeInOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
};

