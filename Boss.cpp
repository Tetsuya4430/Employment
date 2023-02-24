#include "Boss.h"

Boss* Boss::Create(Model* model, Camera* camera, XMFLOAT3 pos)
{
	//3Dオブジェクトのインスタンスを生成
	Boss* instance = new Boss();
	if (instance == nullptr)
	{
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(pos))
	{
		delete instance;
		assert(0);
	}

	//モデルのセット
	if (model)
	{
		instance->SetModel(model);
	}

	//カメラのセット
	if (camera)
	{
		instance->SetCamera(camera);
	}

	return instance;
}

bool Boss::Initialize(XMFLOAT3 pos)
{
	Object3d::position_ = pos;

	

	IntervalTime = 70;
	Speed = 1.0;

	phase_ = Phase::Approach;

	Object3d::Initialize();

	return true;
}

void Boss::Update()
{
	Object3d::Update();

	rotation_.z += 2.0f;

	//更新処理

	if (HP <= 8)
	{
		IntervalTime = 35;
		Speed = 1.5f;
	}

	if (HP <= 0)
	{
		phase_ = Phase::Death;
	}

	switch (phase_)
	{
	case Phase::Approach:
	default:

		//移動
		Object3d::position_.z -= Speed;

		//規定の位置で離脱
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
			//Object3d::position_.y -= 0.2f;
		}

		if (DownTimer % 2 == 0)
		{
			Object3d::position_.x += Shake;
			Shake *= -1.0f;
		}


		break;
	}
}
