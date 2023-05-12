#include "BossParts.h"

std::unique_ptr<BossParts> BossParts::Create(Model* model, Camera* camera, XMFLOAT3 pos)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossParts * instance = new BossParts();
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

	//���j�[�N�|�C���^�𐶐����ĕԂ�
	return std::unique_ptr<BossParts>(instance);
}

bool BossParts::Initialize(XMFLOAT3 pos)
{
	Object3d::position_ = pos;

	Object3d::scale_ = { 3.5f, 3.5f, 3.5f };

	Object3d::Initialize();

	return true;
}

void BossParts::Update()
{
	Object3d::Update();
}
