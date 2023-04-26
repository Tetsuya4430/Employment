#include "Enemy.h"

std::unique_ptr<Enemy> Enemy::Create(Model* model, Camera* camera, XMFLOAT3 pos)
{
	//3Dオブジェクトのインスタンスを生成
	Enemy* instance = new Enemy();
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

	return std::unique_ptr<Enemy>(instance);
}

bool Enemy::Initialize(XMFLOAT3 pos)
{
	Object3d::position_ = pos;

	Object3d::Initialize();
	
	return true;
}

void Enemy::Update()
{
	Object3d::Update();

	if (DownFlag == false)
	{
		switch (phase_)
		{
		case Phase::Approach:
		default:

			//移動
			Object3d::position_.z -= Speed;

			//規定の位置で離脱
			if (Object3d::position_.z <= 30)
			{
				if (Object3d::position_.x == 0)
				{
					if (Object3d::position_.y >= 0)
					{
						phase_ = Phase::LeaveU;
					}

					else if (Object3d::position_.y < 0)
					{
						phase_ = Phase::LeaveD;
					}
				}

				else if (Object3d::position_.x > 0)
				{
					phase_ = Phase::LeaveR;
				}

				else
				{
					phase_ = Phase::LeaveL;
				}
			}
			break;

		case Phase::LeaveR:
			//移動
			Object3d::position_.x -= Speed;
			Object3d::rotation_.z += RotSpeed;

			if (Object3d::position_.x < -100)
			{

				DeathFlag = true;
			}

			break;

		case Phase::LeaveL:
			//移動
			Object3d::position_.x += Speed;
			Object3d::rotation_.z -= RotSpeed;

			if (Object3d::position_.x > 100)
			{
				DeathFlag = true;
			}

			break;

		case Phase::LeaveS:
			//移動

			Object3d::position_.z -= Speed;

			if (Object3d::position_.z <= 0)
			{
				DeathFlag = true;
			}

			break;

		case Phase::LeaveU:
			//移動

			Object3d::position_.y += Speed;

			if (Object3d::position_.y >= 50)
			{
				DeathFlag = true;
			}

			break;

		case Phase::LeaveD:
			//移動

			Object3d::position_.y -= Speed;

			if (Object3d::position_.y <= -50)
			{
				DeathFlag = true;
			}

			break;
		}
	}

	if (DownFlag == true)
	{
		Object3d::VY += Object3d::gravity;

		Object3d::position_.y += Object3d::VY * 0.2f;
	}

}

void Enemy::DefenceEnemyUpdate()
{
	Object3d::Update();

	if (Object3d::position_.z > 30)
	{	
		//移動
		Object3d::position_.z -= DefenceSpeed;
		//回転
		Object3d::rotation_.x -= EnemyRotSpeed;
	}

	if (Object3d::position_.z < 30)
	{
		if (Object3d::position_.x > 0)
		{
			Object3d::position_.x += DefenceSpeed;
			Object3d::rotation_.x = 0.0f;
			Object3d::rotation_.y += EnemyRotSpeed;
		}

		if (Object3d::position_.x < 0)
		{
			Object3d::position_.x -= Speed;
			Object3d::rotation_.x = 0.0f;
			Object3d::rotation_.y -= EnemyRotSpeed;
		}

		if (Object3d::position_.x == 0)
		{
			Object3d::position_.z -= Speed;
			//回転
			Object3d::rotation_.x -= EnemyRotSpeed;
		}
	}

}

