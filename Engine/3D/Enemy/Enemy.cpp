#include "Enemy.h"

std::unique_ptr<Enemy> Enemy::Create(Model* model, Camera* camera, XMFLOAT3 pos)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Enemy* instance = new Enemy();
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

			//�ړ�
			Object3d::position_.z -= Speed;

			//�K��̈ʒu�ŗ��E
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
			//�ړ�
			Object3d::position_.x -= Speed;
			Object3d::rotation_.z += RotSpeed;

			if (Object3d::position_.x < -100)
			{

				DeathFlag = true;
			}

			break;

		case Phase::LeaveL:
			//�ړ�
			Object3d::position_.x += Speed;
			Object3d::rotation_.z -= RotSpeed;

			if (Object3d::position_.x > 100)
			{
				DeathFlag = true;
			}

			break;

		case Phase::LeaveS:
			//�ړ�

			Object3d::position_.z -= Speed;

			if (Object3d::position_.z <= 0)
			{
				DeathFlag = true;
			}

			break;

		case Phase::LeaveU:
			//�ړ�

			Object3d::position_.y += Speed;

			if (Object3d::position_.y >= 50)
			{
				DeathFlag = true;
			}

			break;

		case Phase::LeaveD:
			//�ړ�

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
		//�ړ�
		Object3d::position_.z -= DefenceSpeed;
		//��]
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
			//��]
			Object3d::rotation_.x -= EnemyRotSpeed;
		}
	}

}

