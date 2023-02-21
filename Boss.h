#pragma once
#include "Object3d.h"
class Boss :
    public Object3d
{
public:
	//�s���t�F�[�Y
	enum class Phase
	{
		Approach,	//�ڋ�
		MoveR,		//�ړ�(�E)
		MoveL,		//�ړ�(��)
	};

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static Boss* Create(Model* model, Camera* camera, XMFLOAT3 pos);


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
	void SetHP(int hp) { this->HP = hp; }
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }
	void SetFireTime(bool fireTime) { this->FireTime = fireTime; }

	//getter
	const int& GetHP() { return HP; }
	const bool& GetDeathFlag() { return DeathFlag; }
	const int& GetIntervalTime() { return IntervalTime; }
	const int& GetFireTime() { return FireTime; }

private:
	//���x
	float Speed = 0.5f;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	int DeathFlag = false;

	int IntervalTime = 70;

	int FireTime = 0;

	//�{�X�̗̑�
	int HP = 15;
};


