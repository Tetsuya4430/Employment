#pragma once
#include "Object3d.h"
class Enemy :
    public Object3d
{
public:
	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ�
		LeaveR,		//���E(�E��)
		LeaveL,		//���E(����)
		LeaveS,		//���E(���i)
		LeaveU,		//���E(��)
		LeaveD,		//���E(��)
	};

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<Enemy>Create(Model* model, Camera* camera, XMFLOAT3 pos);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos);

	
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();


	//setter
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }

	void SetIntervalTime(int intervalTime) { this->IntervalTime = intervalTime; }

	void SetFireTime(int fireTime) { this->FireTime = fireTime; }

	//getter
	const bool& GetDeathFlag() { return DeathFlag; }

	const int& GetIntervalTime() { return IntervalTime; }

	const int& GetFireTime() { return FireTime; }

private: // �����o�ϐ�
	// �e�I�u�W�F�N�g
	Enemy* parent_ = nullptr;

	Input* input = nullptr;

	//�Q�[���V�[��

	//���x
	float Speed = 0.4f;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�f�X�t���O
	bool DeathFlag = false;

	//�e�̔��˃C���^�[�o��
	int IntervalTime = 240;

	//���˃^�C�}�[
	int FireTime = 0;
};

