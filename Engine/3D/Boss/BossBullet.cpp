#include "BossBullet.h"

std::unique_ptr<BossBullet> BossBullet::Create(Model* model, Camera* camera, XMFLOAT3 pos, Player* player)
{
	//3Dオブジェクトのインスタンスを生成
	BossBullet* instance = new BossBullet();
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

	//プレイヤーのアドレスを取得
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

	//更新処理

	//時間経過で弾を削除
	if (--DeathTimer <= 0)
	{
		DeathFlag = true;
	}

	Object3d::position_.z -= Speed;
}
