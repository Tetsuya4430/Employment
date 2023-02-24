#include "DefenceObject.h"

std::unique_ptr<DefenceObject> DefenceObject::Create(Model* model, Camera* camera, XMFLOAT3 pos)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	DefenceObject* instance = new DefenceObject();
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

	return std::unique_ptr<DefenceObject>(instance);
}

bool DefenceObject::Initialize(XMFLOAT3 pos)
{
	Object3d::position_ = pos;

	Object3d::Initialize();

	Object3d::SetScale({ 2.0f, 2.0f, 2.0f });

	return true;
}

void DefenceObject::Update(float speed)
{
	Object3d::Update();

	Object3d::rotation_.z += 1.0f;

	switch (phase_)
	{
	case Phase::Approach:
	default:

		//Speed = (float)(rand() % (5 - 1 + 1)) + 1;

		Speed = speed;

		//Speed / 10.0f;

		//�ړ�
		Object3d::position_.z -= (Speed / 10.0f);
		break;

	}
}
