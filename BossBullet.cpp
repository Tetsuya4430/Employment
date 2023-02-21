#include "BossBullet.h"

std::unique_ptr<BossBullet> BossBullet::Create(Model* model, Camera* camera, XMFLOAT3 pos, Player* player)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossBullet* instance = new BossBullet();
	if (instance == nullptr)
	{
		return nullptr;
	}

	//������
	if (!instance->Initialize(pos))
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

	//�v���C���[�̃A�h���X���擾
	if (player)
	{
		instance->SetPlayer(player);
	}

	return std::unique_ptr<BossBullet>(instance);
}

bool BossBullet::Initialize(XMFLOAT3 pos)
{
	Object3d::position_ = pos;

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

	Object3d::position_.z -= Speed;
}
