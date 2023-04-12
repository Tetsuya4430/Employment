#include "BossBullet.h"

std::unique_ptr<BossBullet> BossBullet::Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 PlayerPos)
{
	//3Dオブジェクトのインスタンスを生成
	BossBullet* instance = new BossBullet();
	if (instance == nullptr)
	{
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(pos, PlayerPos))
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

	//更新処理

	//時間経過で弾を削除
	if (--DeathTimer <= 0)
	{
		DeathFlag = true;
	}

	Object3d::position_.x += Speed.x;
	Object3d::position_.y += Speed.y;
	Object3d::position_.z += Speed.z;
}
