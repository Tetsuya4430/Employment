#include "BossParts.h"

std::unique_ptr<BossParts> BossParts::Create(Model* model, Camera* camera, XMFLOAT3 pos)
{
	// 3Dオブジェクトのインスタンスを生成
	BossParts * instance = new BossParts();
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

	//ユニークポインタを生成して返す
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
