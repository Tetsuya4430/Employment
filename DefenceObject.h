#pragma once
#include "Object3d.h"
class DefenceObject :
    public Object3d
{
public:
	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ�
	};

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<DefenceObject>Create(Model* model, Camera* camera, XMFLOAT3 pos);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos);


	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(float speed);


	//setter
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }

	//getter
	const bool& GetDeathFlag() { return DeathFlag; }

private: // �����o�ϐ�
	//���x
	float Speed = 0.4f;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�f�X�t���O
	bool DeathFlag = false;
};

