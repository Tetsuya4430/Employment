#include "BaseScene.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include "DebugText.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Player.h"
#include "Fbx3d.h"
#include "Audio.h"
#include "Input.h"
#include "Bullet.h"
//#include "Reticle.h"
#include "EnemyBullet.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossBullet.h"
#include "ObjectManager.h"
#include "PostEffect.h"
#include "Controller.h"
#include "Particle.h"
//#include "Test.h"

#include <list>
#include <sstream>
#include <fstream>

//class Player;
//class Bullet;
//class EnemyBullet;

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
	using XMVECTOR = DirectX::XMVECTOR;

public:
	static const int debugTextTexNumber = 0;

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

	//ボスの初期化処理
	void BossInit();

	//敵の更新関数
	void EnemyUpdate(XMFLOAT3 enemyPos);

	//プレイヤーの攻撃
	void Attack(XMFLOAT3 StartPos);

	//敵の攻撃
	void EnemyAttack(XMFLOAT3 EnemyPos);

	//ボスの攻撃
	void BossAttack(XMFLOAT3 BossPos);

	//敵の発生データ読み込み
	void LoadEnemyPopData();

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	//パーティクル発生関数
	//void CreateParticleInfo(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale);

	//円と円の当たり判定
	bool CheckCollision(XMFLOAT3 Object1, XMFLOAT3 Object2, float R1, float R2);

	//スプライト読み込み
	void LoadSprite();

	//スプライト更新
	void UpdateSprite();

	//スプライト描画
	void DrawSprite();

	//スタート演出
	void Start();

private:
	//定数
	const int MAX_Object = 10;

private:
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//WindowsAPI
	//WinApp* winApp =  nullptr;

	//デバッグテキスト
	DebugText debugText;

	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	Model* model_Bullet = nullptr;
	Model* model_EnemyBullet = nullptr;
	Model* model_Enemy = nullptr;
	Model* model_Boss = nullptr;
	Model* model_sphere = nullptr;
	Model* model_reticle = nullptr;

	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;

	Input* input = nullptr;

	Camera* camera = nullptr;
	PostEffect* postEffect = nullptr;

	Audio* audio = Audio::GetInstance();

	//弾
	std::list<std::unique_ptr<Bullet>> bullets;

	//コア弾
	std::list<std::unique_ptr<Bullet>> CoreR_bullets;
	std::list<std::unique_ptr<Bullet>> CoreL_bullets;


	//敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemybullets;
	
	//敵
	std::list<std::unique_ptr<Enemy>> enemys;

	int EnemyFire = 0;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//ボスの弾
	//std::list<std::unique_ptr<BossBullet>> bossbullets;
	std::list<std::unique_ptr<BossBullet>> bossbullets;

	Object3d* CelestialSphere = nullptr;

	//プレイヤー
	Player* P = nullptr;
	Player* CoreR = nullptr;
	Player* CoreL = nullptr;
	//Reticle* Reticle = nullptr;
	//Bullet* B = nullptr;

	//ボス
	//St1_Boss* Boss = nullptr;
	Boss* Boss = nullptr;
	int BossFire = 0;
	
	//パーティクル
	Particle* particle = nullptr;
	Particle* EnemyPart = nullptr;


	//Player
	Player* player = nullptr;
	Player* Satellite_R = nullptr;
	Player* Satellite_L = nullptr;

	//UI
	Sprite* sprite = nullptr;
	Sprite* UI = nullptr;
	Sprite* HP_0 = nullptr;
	Sprite* HP_1 = nullptr;
	Sprite* HP_2 = nullptr;
	Sprite* HP_3 = nullptr;
	Sprite* HP_4 = nullptr;
	Sprite* HP_5 = nullptr;
	Sprite* Stage_1 = nullptr;
	Sprite* Go = nullptr;
	Sprite* Rule = nullptr;
	Sprite* LoadBG = nullptr;

	//ボスHPスプライト
	Sprite* BossHP_0 = nullptr;
	Sprite* BossHP_1 = nullptr;
	Sprite* BossHP_2 = nullptr;
	Sprite* BossHP_3 = nullptr;
	Sprite* BossHP_4 = nullptr;
	Sprite* BossHP_5 = nullptr;
	Sprite* BossHP_6 = nullptr;
	Sprite* BossHP_7 = nullptr;
	Sprite* BossHP_8 = nullptr;
	Sprite* BossHP_9 = nullptr;
	Sprite* BossHP_10 = nullptr;
	Sprite* BossHP_11 = nullptr;
	Sprite* BossHP_12 = nullptr;
	Sprite* BossHP_13 = nullptr;
	Sprite* BossHP_14 = nullptr;
	Sprite* BossHP_15 = nullptr;

	//プレイヤー経験値スプライト
	Sprite* ExpBar_0 = nullptr;
	Sprite* ExpBar_1 = nullptr;
	Sprite* ExpBar_2 = nullptr;
	Sprite* ExpBar_3 = nullptr;
	Sprite* ExpBar_4 = nullptr;
	Sprite* ExpBar_5 = nullptr;
	Sprite* Level_1 = nullptr;
	Sprite* Level_2 = nullptr;
	Sprite* Level_3 = nullptr;

	//ボス登場演出UI
	Sprite* BossUI_U = nullptr;
	Sprite* BossUI_U_2 = nullptr;
	Sprite* BossUI_D = nullptr;
	Sprite* BossUI_D_2 = nullptr;
	Sprite* Warning = nullptr;


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

	//ボスの弾発射タイマー
	int BossBulletTimer = 0;

	//ボスの待機フラグ
	bool BossFlag = false;

	//ボス登場演出フラグ
	bool BossFlag_S = false;
	bool BossFlag_E = false;

	//ボス演出UI回帰用フラグ
	bool EndUIFlag = false;
	
	//ボスUI描画フラグ
	bool BossUIDrawFlag = false;

	//Warningフラグ
	bool WarningFlag = false;

	//ロードフラグ
	bool LoadFlag = false;

	//ボスの待機タイマー
	int BossTimer = 128;

	//csvファイルから敵の座標格納用変数
	XMFLOAT3 Num = {0, 0, 0};

	//ボス撃破からの演出タイマー
	int WaitTimer = 0;

	//ゲームスタート演出用
	int StartTimer = 0;

	public:
	//弾の発射間隔
	static const int BulletInterval = 60;

	//ボスの弾発射間隔
	static const int BossBulletInterval = 5;

	//レティクル座標
	XMFLOAT3 ReticlePos = { 0, 0, 0 };

	//テスト
	int Timer = 60;

	//ボス登場フラグ
	bool BossHPFlag = false;

	//ボス体力タイマー
	int BossHPTimer = 120;

	XMFLOAT2 BarSize = { 100, 200 };


	//プレイヤー関係
	XMFLOAT3 PlayerPos = { 0, 0, 0 };
	XMFLOAT3 SatellitePos_R = { 0, 0, 0 };
	XMFLOAT3 SatellitePos_L = { 0, 0, 0 };

	//敵
	XMFLOAT3 EnemyPos = { 0, 0, 0 };

	//オーディオ音量
	float Attack_Volume = 0.5f;
};

