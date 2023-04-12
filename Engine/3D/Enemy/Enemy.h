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

	void DefenceEnemyUpdate();


	//setter
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }

	void SetDownFlag(bool downFlag) { this->DownFlag = downFlag; }

	void SetIntervalTime(int intervalTime) { this->IntervalTime = intervalTime; }

	void SetFireTime(int fireTime) { this->FireTime = fireTime; }

	//getter
	const bool& GetDeathFlag() { return DeathFlag; }

	const bool& GetDownFlag() { return DownFlag; }

	const int& GetIntervalTime() { return IntervalTime; }

	const int& GetFireTime() { return FireTime; }

	const XMFLOAT4& GetEnemyDeathPartColor() { return EnemyDeathPartColor; }

private: // �����o�ϐ�
	//���x
	float Speed = 0.4f;

	//��]���x
	float RotSpeed = 5.0f;

	//��]�G�̉�]���x
	float EnemyRotSpeed = 10.0f;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�f�X�t���O
	bool DeathFlag = false;

	//�����t���O
	bool DownFlag = false;

	//�e�̔��˃C���^�[�o��
	int IntervalTime = 240;

	//���˃^�C�}�[
	int FireTime = 0;

	//�p�[�e�B�N���J���[
	XMFLOAT4 EnemyDeathPartColor = { 0.972f, 0.654f, 0.286f, 1.0f };
};

