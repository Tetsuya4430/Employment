#pragma once
#include "Object3d.h"
class Player :
    public Object3d
{
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static Player* Create(Model* model, Camera* camera);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	//setter
	void SetLelelflag(bool Lelelflag) { this->LevelFlag = Lelelflag; }

	//���x���t���O�̃Q�b�^�[
	const bool& GetLelelflag() { return  LevelFlag; }

	//HP�̃Q�b�^�[
	const float& GetHP() { return  HP; }

	const float& GetMAXHP() { return  MAX_HP; }

private:
	//�v���C���[�̈ړ��X�s�[�h
	float Speed = 0.5f;
	float K = 0.5f;

	//�v���C���[�̈ړ��W��
	float Mag = 1.0f;

	//�v���C���[�̈ړ��ʕۑ�
	XMFLOAT2 Old = { 0.0f, 0.0f };

	//�v���C���[��]���x
	float RotlimR = -15.0f;
	float RotlimL = 15.0f;

	//�v���C���[�̈ړ��t���O
	int MoveFlag = 0;

	//�v���C���[�C�[�W���O�p�ϐ�
	float dx = 0;
	float PointPos = -50;

	//����J�n���̎��@�̍��W���i�[����ϐ�
	float AvoidStart_X = 0;

	//����t���O
	bool Avoidanceflag_X = false;
	bool Avoidanceflag_Y = false;

	//����^�C�}�[
	int AvoidanceTimer_X = 0;
	int AvoidanceTimer_Y = 0;

	//��������萔
	const int AvoidDistance_X = 20;
	const int AvoidDistance_Y = 10;

	//��]�����萔
	const float RoleDistance = 360;

	//��]�i�[�ϐ�
	float RolePos = 0;

	float rx = 0;

	//����������t���O
	bool RotFlag_R = false;
	bool RotFlag_L = false;

	//����^�C��
	const float AvoidCount = 10.0f;

	//���鎞��
	int Timer = 0;

	//�v���C���[�̌X������l
	float RotLimit = 15.0f;

	//��]��
	float RotValue = 1.0f;

	//�ړ�����
	bool moveR = false;
	bool moveL = false;
	bool moveD = false;
	bool moveU = false;

public:
	// ���[�J�����W
	//XMFLOAT3 position_ = { 0,0,0 };

	//���e�B�N�����W
	XMFLOAT3 ReticlePos = { 0, 0, 0 };

	// X,Y,Z�����̃��[�J����]�p
	//XMFLOAT3 rotation_ = { 0,0,0 };

	//�v���C���[�̍ő�HP
	float MAX_HP = 10;

	//�v���C���[��HP
	float HP = 10;

	//�v���C���[�̃��x��
	int Level = 1;

	//�v���C���[�̌o���l
	int EXP = 0;

	//�ړ��\�t���O
	bool MoveCanFlag = false;

	//�_���[�W�t���O
	bool DamageFlag = false;

	//���x���A�b�v�t���O
	bool LevelFlag = false;
};

