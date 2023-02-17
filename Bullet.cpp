#include "Bullet.h"

std::unique_ptr<Bullet> Bullet::Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 ReticlePos)
{
    //3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
    Bullet* instance = new Bullet();
    if (instance == nullptr)
    {
        return nullptr;
    }

    //������
    if (!instance->Initialize(pos, ReticlePos))
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
	return std::unique_ptr<Bullet>(instance);
}

bool Bullet::Initialize(XMFLOAT3 pos, XMFLOAT3 PointPos)
{
    Object3d::position_ = pos;

    Speed.x = (PointPos.x - Object3d::position_.x) / Count;
    Speed.y = (PointPos.y - Object3d::position_.y) / Count;
    Speed.z = ((PointPos.z + 30.0f) - Object3d::position_.z) / Count;

    Object3d::Initialize();

	return true;
}

void Bullet::Update(XMFLOAT3 pos)
{
    Object3d::Update();

    //���Ԍo�߂Œe���폜
    if (--DeathTimer <= 0)
    {
        DeathFlag = true;
    }

    //�e�̈ړ�
    /*position_B.x += Speed.x;
    position_B.y += Speed.y;
    position_B.z += Speed.z;*/

    Object3d::position_.z += 5.0f;
}
