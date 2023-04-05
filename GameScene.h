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
#include "EnemyBullet.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossBullet.h"
#include "ObjectManager.h"
#include "PostEffect.h"
#include "Controller.h"
#include "Particle.h"
#include "DefenceObject.h"
#include "ImGuiManager.h"

#include <list>
#include <sstream>
#include <fstream>


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

	//ダメージエフェクト
	void Damage();

	//ゲームオーバー演出
	void GameOver();

	//隕石の挙動
	void MeteorMove();

	//ボスの死亡エフェクト
	void BossDeath();

	//タイトル演出
	void MoveTitle();

	//デバッグテキスト描画用関数
	void DebagText();

	void HpBarMove();

	void EnemyDown(XMFLOAT3 EnemyPos);

	void CameraMove();

	//UIアルファ値操作関数
	void UI_AlphaIncriment(float color_w);
	void UI_AlphaDecriment(float color_w);

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
	Model* model_Moon = nullptr;
	Model* model_Mars = nullptr;
	Model* model_Neptune = nullptr;
	Model* model_Wall = nullptr;
	Model* model_Meteor = nullptr;
	Model* model_Station = nullptr;
	Model* model_SpaceStation = nullptr;
	Model* model_reticle = nullptr;
	Model* model_Shooting = nullptr;
	Model* model_Force = nullptr;

	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;

	Input* input = nullptr;

	//カメラ
	Camera* camera = nullptr;

	XMFLOAT3 CameraDefault = { 0, 0, -50.0f };

	//揺れ幅
	XMFLOAT3 Shake = { 2.0f, 0.0f, 0.0f };

	int ShakeTimer = 0;


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

	std::list<std::unique_ptr<DefenceObject>> objects;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//ボスの弾
	//std::list<std::unique_ptr<BossBullet>> bossbullets;
	std::list<std::unique_ptr<BossBullet>> bossbullets;

	//3Dオブジェクト
	Object3d* CelestialSphere = nullptr;
	Object3d* Moon = nullptr;
	Object3d* Mars = nullptr;
	Object3d* Neptune = nullptr;
	Object3d* Wall = nullptr;
	Object3d* Station = nullptr;
	Object3d* SpaceStation = nullptr;
	Object3d* Shooting = nullptr;
	Object3d* Force = nullptr;
	Object3d* CameraObject = nullptr;

	Object3d* test = nullptr;

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
	//Particle* EnemyPart = nullptr;
	//Particle* ShotPart = nullptr;
	Particle* particle_Red = nullptr;

	Particle* Testpart = nullptr;


	//Player
	Player* player = nullptr;
	Player* Satellite_R = nullptr;
	Player* Satellite_L = nullptr;

	XMFLOAT3 PlayerScale = { 0.5f, 0.5f, 0.5f };
	XMFLOAT3 SatelliteRange = { 6.0, 1.0, 0.0 };

	//UI
	Sprite* sprite = nullptr;
	Sprite* UI = nullptr;
	Sprite* Stage_1 = nullptr;
	Sprite* Go = nullptr;
	Sprite* Rule = nullptr;
	Sprite* LoadBG = nullptr;
	Sprite* DamageEffect = nullptr;
	Sprite* StartUI = nullptr;
	Sprite* HPBar = nullptr;
	Sprite* EmpBar = nullptr;
	Sprite* BossHPBar = nullptr;
	Sprite* PlayerFrame = nullptr;
	Sprite* BossFrame = nullptr;
	Sprite* EmpBossBar = nullptr;

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

	//ImGui
	ImGuiManager* Imgui = nullptr;

	//プレイヤーのヒットフラグ
	bool HitFlag = false;

	//連射タイマー
	int ShotTimer = 0;


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

	//ボス死亡演出のパーティクルの発生カウント
	int BossPartTimer = 0;
	int PartCount = 0;

	//csvファイルから敵の座標格納用変数
	XMFLOAT3 Num = {0, 0, 0};

	//ボス撃破からの演出タイマー
	int WaitTimer = 0;

	//ゲームスタート演出用
	int StartTimer = 0;

	//3dオブジェクト用変数
	XMFLOAT3 MoonPos = { 0, 0, 0 };
	XMFLOAT3 MoonRot = { 0, 0, 0 };

	XMFLOAT3 MarsPos = { 0, 0, 0 };
	XMFLOAT3 MarsRot = { 0, 0, 0 };

	XMFLOAT3 NeptunePos = { 0, 0, 0 };
	XMFLOAT3 NeptuneRot = { 0, 0, 0 };

	XMFLOAT3 StationPos = { 0, 0, 0 };
	XMFLOAT3 StationRot = { 0, 0, 0 };

	//天球スケール
	XMFLOAT3 ShereScale = { 900, 900, 900 };

	//月スケール
	XMFLOAT3 MoonScale = { 60, 60, 60 };
	//月座標
	XMFLOAT3 MoonPosition = { 350, 150, 400 };

	//火星スケール
	XMFLOAT3 MarsScale = { 20, 20, 20 };
	//火星座標
	XMFLOAT3 MarsPosition = { 100, -100, 500 };

	//海王星スケール
	XMFLOAT3 NeptuneScale = { 30, 30, 30 };
	//海王星座標
	XMFLOAT3 NeptunePosition = { -200, 150, 600 };

	//宇宙ステーションスケール
	XMFLOAT3 StationScale = { 50, 50, 50 };
	//宇宙ステーション座標
	XMFLOAT3 StationPosition = { -300, -50, 500 };
	//宇宙ステーション回転
	XMFLOAT3 StationRotation = { -60, 0, 0 };

	//テキスト座標(Shooting)
	XMFLOAT3 DefaultShootingScale = { 8, 8, 8 };
	XMFLOAT3 DefaultShootingPos = { -20, 10, 0 };
	//テキスト座標(Force)
	XMFLOAT3 DefaultForceScale = { 8, 8, 8 };
	XMFLOAT3 DefaultForcePos = { 0, 0, 0 };

	//スタート演出UI用変数(Stage1)
	XMFLOAT2 DefaultStage1Pos = { 1280, 285 };
	//目的座標変数
	XMFLOAT3 Stage1PointPos = { 460, 285 , 0};

	//スタート演出UI用変数(Go)
	XMFLOAT2 DefaultGoPos = { 0, -720 };
	//目的座標変数
	XMFLOAT3 GOPointPos = { 0, 0, 0};

	//ボス登場時UIのカラー変数
	XMFLOAT4 BossUIColor = { 1, 1, 1, 0.7f };
	//ボス登場時UI用座標
	XMFLOAT2 BossUIPos_1 = { 0, 100 };
	XMFLOAT2 BossUIPos_2 = { 1280, 100 };
	//ボス登場時UI用目的座標
	XMFLOAT3 BossUIPointPos_1 = { 1280, 0, 0 };
	XMFLOAT3 BossUIPointPos_2 = { 0, 0, 0 };

	//カメラオブジェクト用変数
	XMFLOAT3 DefaultCameraObjPos = { 0, 0, -50.0f };
	XMFLOAT3 DefaultCameraObjScale = { 1, 1, 1 };

	//タイマーリセット用変数
	int TimerReset = 0;
	//ボスタイマーリセット
	int DefaultBossTimer = 128;

	//ステージUI移動からゲームスタートUI移動までのレンジタイム
	int RangeTime = 0;

	//初期アルファ値
	//MAX
	float DefaultAlpha_Max = 1.0f;

	float VY = 0.0f;	//Y方向の速度
	const float gravity = -9.8f / 60.0f;	//重力

	bool DamageEffectflag = false;

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

	XMFLOAT2 PlayerHPSize = {800, 30};
	float HPdiv = 0.0f;
	XMFLOAT2 PlayerNowHP = {1, PlayerHPSize.y};

	XMFLOAT2 BossHPSize = { 800, 30 };
	float BossHPdiv = 0.0f;
	XMFLOAT2 BossNowHP = { 1, BossHPSize.y };

	//フレーム
	XMFLOAT2 FrameSize = { 820, 50 };

	//プレイヤー関係
	XMFLOAT3 PlayerPos = { 0, 0, 0 };
	XMFLOAT3 PlayerRot = { 0, 0, 0 };
	XMFLOAT3 SatellitePos_R = { 0, 0, 0 };
	XMFLOAT3 SatellitePos_L = { 0, 0, 0 };

	XMFLOAT3 TestPos = { 0, 0, 0 };

	XMFLOAT3 testStart = { 0, 0, 0 };
	XMFLOAT3 testEnd = { 20, 20, 40 };

	XMFLOAT3 PartSpeed = { 0, 0, 10 };
	XMFLOAT3 WarningPartSpeed = { 5, 5, 3 };
	float PartVel = 10.0f;

	//敵
	XMFLOAT3 EnemyPos = { 0, 0, 0 };

	//隕石発生タイマー
	int MeteorTimer = 0;

	XMFLOAT3 MeteorPos = { 0, 0, 0 };

	//オーディオ音量
	float Attack_Volume = 0.5f;
	float GameBGM_Volume = 0.1f;

	//ゲームスタートフラグ
	bool GameStart = false;

	//タイトルテキスト用座標
	XMFLOAT3 ShootingPos = { 0, 0, 0 };

	XMFLOAT3 ForcePos = { 0, 0, 0 };

	int Count = 0;
	float Move_F = 0.0f;

	XMFLOAT3 CameraPos = { 0, 0, -50.0f };

	//カメラの視点移動制限
	float CameraLimit = 1.0f;

	float CameraMoveValue = 0.1f;

	//レベルタイマー
	int LevelCount = 0;

	//UI演出待機カウント
	int RangeCount = 100;

	//ボス撃破時の待機カウント
	int WaitCount = 120;

	//ゲームオーバー時の待機カウント
	int WaitCount_GameOver = 100;

	//タイマーインクリメント変数
	int TimerIncrimentValue = 1;

	//ゼロ変数
	int Value_Zero = 0;

	//---UIのアルファ値設定用---//
	//アルファ値最大値
	const float MAX_ALPHA = 1.0f;
	//アルファ値の最小値
	const float MIN_ALPHA = 0.5f;
};

