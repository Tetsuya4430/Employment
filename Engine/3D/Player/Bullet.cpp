#include "Bullet.h"

std::unique_ptr<Bullet> Bullet::Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 ReticlePos)
{
    //3Dオブジェクトのインスタンスを生成
    Bullet* instance = new Bullet();
    if (instance == nullptr)
    {
        return nullptr;
    }

    //初期化
    if (!instance->Initialize(pos, ReticlePos))
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
	return std::unique_ptr<Bullet>(instance);
}

bool Bullet::Initialize(XMFLOAT3 pos, XMFLOAT3 PointPos)
{
    Object3d::position_ = pos;

    Object3d::Initialize();

	return true;
}

void Bullet::Update(XMFLOAT3 pos)
{
    Object3d::Update();

    //時間経過で弾を削除
    if (--DeathTimer <= 0)
    {
        DeathFlag = true;
    }

    Object3d::position_.z += Speed.z;
}
