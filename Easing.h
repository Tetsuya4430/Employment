#pragma once
#include <DirectXMath.h>

class Easing
{
public:
	/// <summary>
	/// �C�[�Y�C��
	/// </summary>
	/// <param name="Move">�ړ���</param>
	/// <param name="BasePos">�ŏ��̈ʒu</param>
	/// <param name="MoveTime">�ړ�����</param>
	/// <param name="Time">���݂̌o�ߎ���</param>
	/// <returns></returns>
	static float EaseIn(float Move, float BasePos, float MoveTime, float Time);
};

