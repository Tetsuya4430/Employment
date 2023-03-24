#pragma once
#include <DirectXMath.h>
class Vector3
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	//��Ԋ֐�
	static XMFLOAT3 lerp(XMFLOAT3 position, const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	const XMFLOAT3 easeIn(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	const XMFLOAT3 easeOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
	const XMFLOAT3 easeInOut(const XMFLOAT3& start, const XMFLOAT3& end, const float t);
};

