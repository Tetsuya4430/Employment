#pragma once
#include "Object3d.h"
class Enemy :
    public Object3d
{
public:
	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近
		LeaveR,		//離脱(右側)
		LeaveL,		//離脱(左側)
		LeaveS,		//離脱(直進)
		LeaveU,		//離脱(上)
		LeaveD,		//離脱(下)
	};

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<Enemy>Create(Model* model, Camera* camera, XMFLOAT3 pos);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos);

	
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();


	//setter
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }

	void SetIntervalTime(int intervalTime) { this->IntervalTime = intervalTime; }

	void SetFireTime(int fireTime) { this->FireTime = fireTime; }

	//getter
	const bool& GetDeathFlag() { return DeathFlag; }

	const int& GetIntervalTime() { return IntervalTime; }

	const int& GetFireTime() { return FireTime; }

private: // メンバ変数
	// 親オブジェクト
	Enemy* parent_ = nullptr;

	Input* input = nullptr;

	//ゲームシーン

	//速度
	float Speed = 0.4f;

	//フェーズ
	Phase phase_ = Phase::Approach;

	//デスフラグ
	bool DeathFlag = false;

	//弾の発射インターバル
	int IntervalTime = 240;

	//発射タイマー
	int FireTime = 0;
};

