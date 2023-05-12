#include "Boss.h"

#include "Input.h"

std::unique_ptr<Boss> Boss::Create(Model* model, Camera* camera, XMFLOAT3 pos)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Boss* instance = new Boss();
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
	return std::unique_ptr<Boss>(instance);
}

bool Boss::Initialize(XMFLOAT3 pos)
{
	Object3d::position_ = pos;

	Object3d::scale_ = { 2.5f, 2.5f, 2.5f };

	

	IntervalTime = 45;
	Speed = 1.0;

	phase_ = Phase::Approach;

	Object3d::Initialize();

	return true;
}

void Boss::Update()
{
	Object3d::Update();

	rotation_.z += 2.0f;

	if (Input::GetInstance()->PushKey(DIK_L))
	{
		Object3d::position_.x += Speed;
	}

	if (Input::GetInstance()->PushKey(DIK_J))
	{
		Object3d::position_.x -= Speed;
	}

	//�X�V����
	if (DamegeFlag == true)
	{
		DamegeTimer++;
		Object3d::Color = DamegeColor;

		if (DamegeTimer >= 20)
		{
			Object3d::Color = BaseColor;
			DamegeTimer = 0;
			DamegeFlag = false;
		}
	}
	

	if (HP <= 0)
	{
		phase_ = Phase::Death;
	}

	switch (phase_)
	{
	case Phase::Approach:
	default:

		//�ړ�
		Object3d::position_.z -= Speed;

		//�K��̈ʒu�ŗ��E
		if (Object3d::position_.z <= 30)
		{
			phase_ = Phase::MoveR;
		}
		break;

	case Phase::MoveR:
		if (Object3d::position_.x < 50 && HP > 0)
		{
			Object3d::position_.x += Speed;
		}

		if (Object3d::position_.x >= 50 && HP > 0)
		{
			phase_ = Phase::MoveL;
		}

		/*if (HP <= 30 && HP > 0)
		{
			LastTimer++;

			
		}*/

		break;

	case Phase::MoveL:
		if (Object3d::position_.x > -50 && HP > 0)
		{
			Object3d::position_.x -= Speed;
		}


		if (Object3d::position_.x <= -50 && HP > 0)
		{
			phase_ = Phase::MoveR;
		}

		break;

	case Phase::Death:

		DownTimer += 1;

		if (DownTimer <= 60)
		{
			Object3d::position_.y -= 0.2f;
		}

		if (DownTimer % 2 == 0)
		{
			Object3d::position_.x += Shake;
			Shake *= -1.0f;
		}


		break;

	/*case Phase::Last:

		LastTimer++;

		LastTimer

		break;*/
	}
}

void Boss::Damage()
{
	/*int Time;

	Time++;*/

	SetColor(DamegeColor);

	/*if (Time >= 20)
	{
		SetColor(Object3d::BaseColor);
	}*/
}
