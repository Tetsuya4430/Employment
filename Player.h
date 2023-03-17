#pragma once
#include "Object3d.h"
class Player :
    public Object3d
{
public:
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static Player* Create(Model* model, Camera* camera);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	//setter
	void SetLelelflag(bool Lelelflag) { this->LevelFlag = Lelelflag; }

	//レベルフラグのゲッター
	const bool& GetLelelflag() { return  LevelFlag; }

	//HPのゲッター
	const float& GetHP() { return  HP; }

	const float& GetMAXHP() { return  MAX_HP; }

private:
	//プレイヤーの移動スピード
	float Speed = 0.5f;
	float K = 0.5f;

	//プレイヤーの移動係数
	float Mag = 1.0f;

	//プレイヤーの移動量保存
	XMFLOAT2 Old = { 0.0f, 0.0f };

	//プレイヤー回転限度
	float RotlimR = -15.0f;
	float RotlimL = 15.0f;

	//プレイヤーの移動フラグ
	int MoveFlag = 0;

	//プレイヤーイージング用変数
	float dx = 0;
	float PointPos = -50;

	//回避開始時の自機の座標を格納する変数
	float AvoidStart_X = 0;

	//回避フラグ
	bool Avoidanceflag_X = false;
	bool Avoidanceflag_Y = false;

	//回避タイマー
	int AvoidanceTimer_X = 0;
	int AvoidanceTimer_Y = 0;

	//回避距離定数
	const int AvoidDistance_X = 20;
	const int AvoidDistance_Y = 10;

	//回転距離定数
	const float RoleDistance = 360;

	//回転格納変数
	float RolePos = 0;

	float rx = 0;

	//回避旋回方向フラグ
	bool RotFlag_R = false;
	bool RotFlag_L = false;

	//回避タイム
	const float AvoidCount = 10.0f;

	//滑る時間
	int Timer = 0;

	//プレイヤーの傾き上限値
	float RotLimit = 15.0f;

	//回転量
	float RotValue = 1.0f;

	//移動方向
	bool moveR = false;
	bool moveL = false;
	bool moveD = false;
	bool moveU = false;

public:
	// ローカル座標
	//XMFLOAT3 position_ = { 0,0,0 };

	//レティクル座標
	XMFLOAT3 ReticlePos = { 0, 0, 0 };

	// X,Y,Z軸回りのローカル回転角
	//XMFLOAT3 rotation_ = { 0,0,0 };

	//プレイヤーの最大HP
	float MAX_HP = 10;

	//プレイヤーのHP
	float HP = 10;

	//プレイヤーのレベル
	int Level = 1;

	//プレイヤーの経験値
	int EXP = 0;

	//移動可能フラグ
	bool MoveCanFlag = false;

	//ダメージフラグ
	bool DamageFlag = false;

	//レベルアップフラグ
	bool LevelFlag = false;
};

