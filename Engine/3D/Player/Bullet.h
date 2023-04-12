#pragma once
#include "Object3d.h"
class Bullet :
    public Object3d
{
	public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<Bullet>Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 ReticlePos);


	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos, XMFLOAT3 PointPos);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(XMFLOAT3 pos);


	//setter
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }

	//�e���̃Z�b�^�[
	void SetSpeed(XMFLOAT3 speed) { this->Speed = speed; }

	//�e�F�̃Z�b�^�[
	void SetBulletColor(XMFLOAT4 bulletColor) { this->BulletColor = bulletColor; }

	//getter
	//�f�X�t���O�̃Q�b�^�[
	const bool& GetDeathFlag() { return DeathFlag; }

	//�e���̃Q�b�^�[
	const XMFLOAT3 GetSpeed() { return Speed; }

	//�J���[�̃Q�b�^�[
	const XMFLOAT4 GetBulletColor() { return BulletColor; }
	const XMFLOAT4 GetCounterColor() { return CounterColor; }

private:
	//�e�̎���
	static const int LifeTimer = 60 * 2;

	//���x
	XMFLOAT3 Speed = { 0.0f, 0.0f, 5.0f };

	//�J�E���g
	float Count = 50;

	//�e�̃f�X�^�C�}�[
	int DeathTimer = LifeTimer;

	//�e�̃f�X�t���O
	bool DeathFlag = false;

	//�e�̐F
	XMFLOAT4 BulletColor = { 1.0, 0.654, 0.1, 1.0 };
	XMFLOAT4 CounterColor = { 1.0, 0.1, 0.1, 1.0 };
};

