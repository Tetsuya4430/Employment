#include "BossBullet.h"

std::unique_ptr<BossBullet> BossBullet::Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 PlayerPos)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossBullet* instance = new BossBullet();
	if (instance == nullptr)
	{
		return nullptr;
	}

	//������
	if (!instance->Initialize(pos, PlayerPos))
	{
		delete instance;
		assert(0);
	}

	//���f���̃Z�b�g
	if (model)
	{
		instance->SetModel(model);
	}

	//�J�����̃Z�b�g
	if (camera)
	{
		instance->SetCamera(camera);
	}


	return std::unique_ptr<BossBullet>(instance);
}

bool BossBullet::Initialize(XMFLOAT3 pos, XMFLOAT3 PlayerPos)
{
	Object3d::position_ = pos;

	Speed.x = (PlayerPos.x - Object3d::position_.x) / Count;
	Speed.y = (PlayerPos.y - Object3d::position_.y) / Count;
	Speed.z = (PlayerPos.z - Object3d::position_.z) / Count;

	Object3d::Initialize();

	return true;
}

void BossBullet::Update(XMFLOAT3 PlayerPos, XMFLOAT3 EnemyPos)
{
	Object3d::Update();

	//�X�V����

	//���Ԍo�߂Œe���폜
	if (--DeathTimer <= 0)
	{
		DeathFlag = true;
	}

	Object3d::position_.x += Speed.x;
	Object3d::position_.y += Speed.y;
	Object3d::position_.z += Speed.z;
}
