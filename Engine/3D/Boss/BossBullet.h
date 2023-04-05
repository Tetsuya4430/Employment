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
	static std::unique_ptr<BossBullet>Create(Model* model, Camera* camera, XMFLOAT3 pos, Player* player);


	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos);

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


	//�v���C���[
	Player* player_ = nullptr;

	//�e�̎���
	static const int LifeTimer = 60 * 2;

	//���x
	float Speed = 5.0f;

	//�ړ��p�x�N�g��
	XMFLOAT3 Vec = { 0.0f, 0.0f, 0.0f };

	//�e�̃f�X�^�C�}�[
	int DeathTimer = LifeTimer;

	//�e�̃f�X�t���O
	bool DeathFlag = false;
};

