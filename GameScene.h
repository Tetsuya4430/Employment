

#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Fbx3d.h"
#include "Input.h"
#include "ObjectManager.h"
#include "PostEffect.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GameScene : public BaseScene
{
public:
	// コンストクラタ
	GameScene();

	// デストラクタ
	~GameScene();

	//初期化
	void Initialize(/*DirectXCommon* dxCommon*/) ;

	//終了処理
	void Finalize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

private:
	//定数
	const int MAX_Object = 10;

private:
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Sprite* sprite = nullptr;
	Model* model_1 = nullptr;
	Model* model_2 = nullptr;

	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;

	Input* input = nullptr;

	Camera* camera = nullptr;
	PostEffect* postEffect = nullptr;

	std::shared_ptr<Object3d> object3d_1;
	std::shared_ptr<Object3d> BulletObj;
	std::shared_ptr<Object3d> Player;
	std::unique_ptr<ObjectManager> objectManager_1;
	std::unique_ptr<ObjectManager> objectManager_2;
	std::unique_ptr<ObjectManager> objectManager_3;

	float MoveVec = 1.0f;

	int BulletFlag = 0;
};

