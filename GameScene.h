#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Fbx3d.h"
#include "Input.h"
#include "ObjectManager.h"
#include "PostEffect.h"
#include "Controller.h"

class Player;
/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GameScene : public BaseScene
{
public:
	// �R���X�g�N���^
	GameScene();

	// �f�X�g���N�^
	~GameScene();

	//������
	void Initialize(/*DirectXCommon* dxCommon*/) ;

	//�I������
	void Finalize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

private:
	//�萔
	const int MAX_Object = 10;

private:
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Sprite* sprite = nullptr;
	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	Model* model_Bullet = nullptr;
	Model* model_Enemy = nullptr;

	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;

	Input* input = nullptr;

	Camera* camera = nullptr;
	PostEffect* postEffect = nullptr;


	/*std::shared_ptr<Object3d> Empty;
	std::shared_ptr<Object3d> BulletObj;
	std::shared_ptr<Object3d> player;
	std::shared_ptr<Object3d> Enemy;
	std::unique_ptr<ObjectManager> objectManager_1;
	std::unique_ptr<ObjectManager> objectManager_2;
	std::unique_ptr<ObjectManager> objectManager_3;
	std::unique_ptr<ObjectManager> objectManager_4;*/


	Object3d* Empty = nullptr;
	Object3d* BulletObj = nullptr;
	Object3d* player = nullptr;
	Object3d* Enemy = nullptr;

	Player* P = nullptr;

	float MoveVec = 1.0f;

	int BulletFlag = 0;

	int MoveFlag = 0;
};

