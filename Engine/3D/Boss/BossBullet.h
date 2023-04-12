#pragma once
#include "Object3d.h"

//���@�N���X�̑O���錾
class Player;

class BossBullet :
    public Object3d
{
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<BossBullet>Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 PlayerPos);


	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos, XMFLOAT3 PlayerPos);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(XMFLOAT3 PlayerPos, XMFLOAT3 EnemyPos);

	//setter
	//�v���C���[�̃Z�b�^�[
	void SetPlayer(Player* player) { player_ = player; }

	//getter
	//�f�X�t���O�̃Q�b�^�[
	bool DeathGetter() const { return DeathFlag; }

	//�e���̃Q�b�^�[
	const XMFLOAT3& GetSpeed() { return Speed; }


	//�v���C���[
	Player* player_ = nullptr;

	//�e�̎���
	static const int LifeTimer = 60 * 2;

	//���x
	XMFLOAT3 Speed = { 0.0f, 0.0f, 0.0f };

	//�ړ��p�x�N�g��
	XMFLOAT3 Vec = { 0.0f, 0.0f, 0.0f };

	//�e�̃f�X�^�C�}�[
	int DeathTimer = LifeTimer;

	//�e�̃f�X�t���O
	bool DeathFlag = false;

	//���@�_���e�p
	float PointPos = 0;
	float Count = 50.0f;
};

