#include "BaseScene.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Fbx3d.h"
#include "Input.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "Enemy.h"
#include "ObjectManager.h"
#include "Particle.h"
#include "Part.h"
#include "PostEffect.h"
#include "Controller.h"


#include <list>
#include <sstream>
#include <fstream>

class Player;
class Bullet;
class EnemyBullet;
class Enemy;

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GameScene : public BaseScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

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


	//敵の初期化処理
	void EnemyInit();

	//敵の更新関数
	void EnemyUpdate(XMFLOAT3 enemyPos);

	//プレイヤーの攻撃
	void Attack();

	//敵の攻撃
	void EnemyAttack(XMFLOAT3 EnemyPos);

	//敵の発生データ読み込み
	void LoadEnemyPopData();

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	//円と円の当たり判定
	bool CheckCollision(XMFLOAT3 Object1, XMFLOAT3 Object2, float R1, float R2);

private:
	//定数
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

	//弾
	std::list<std::unique_ptr<Bullet>> bullets;

	//敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemybullets;
	
	//敵
	std::list<std::unique_ptr<Enemy>> enemys;

	//敵発生コマンド
	std::stringstream enemyPopCommands;



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
	//Object3d* Enemy = nullptr;

	Player* P = nullptr;
	Bullet* B = nullptr;
	//Enemy* E = nullptr;

	//std::unique_ptr<Enemy> enemy;

	//パーティクル
	Particle* particle = nullptr;

	Part* part = nullptr;


	//敵の座標

	float MoveVec = 1.0f;

	int BulletFlag = 0;

	int MoveFlag = 0;

	//敵の弾発射タイマー
	int EnemyBulletTimer = 0;

	//敵の待機フラグ
	bool EnemyFlag = false;

	//敵の待機タイマー
	int EnemyTimer = 0;

	//csvファイルから敵の座標格納用変数
	XMFLOAT3 Num = {0, 0, 0};

	public:
	//弾の発射間隔
	static const int BulletInterval = 60;


	//テスト
	int Timer = 60;

};

