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
	static const double easeIn(const float time, const float start, const float startToend , float end);

	static const double ease_in_cubic(double x);
};

