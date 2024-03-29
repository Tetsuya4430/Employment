#pragma once
#include "Object3d.h"
class Boss :
    public Object3d
{
public:
	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近
		MoveR,		//移動(右)
		MoveL,		//移動(左)
		Death,		//死亡
		Last,
	};

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<Boss> Create(Model* model, Camera* camera, XMFLOAT3 pos);


	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	//ダメージによるカラー変更
	void Damage();

	//setter
	void SetHP(int hp) { this->HP = hp; }
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }
	void SetDeathEffect(bool deathEffect) { this->DeathEffect = deathEffect; }
	void SetFireTime(bool fireTime) { this->FireTime = fireTime; }
	void SetDamegeFlag(bool damegeFlag) { this->DamegeFlag = damegeFlag; }

	//getter
	const float& GetHP() { return HP; }
	const bool& GetDeathFlag() { return DeathFlag; }
	const bool& GetDeathEffect() { return DeathEffect; }
	const int& GetIntervalTime() { return IntervalTime; }
	const int& GetFireTime() { return FireTime; }
	const float& GetMAXHP() { return MAXHP; }
	const XMFLOAT4& GetBossParticleColor() { return BossParticleColor; }
	const bool& GetDamegeFlag() { return DamegeFlag; }

private:
	//速度
	float Speed = 0.5f;

	//フェーズ
	Phase phase_ = Phase::Approach;

	bool DeathFlag = false;

	bool DeathEffect = false;

	int IntervalTime = 45;

	float Shake = 0.5f;

	int FireTime = 0;

	int DownTimer = 0;

	int LastTimer = 0;

	//ボスの体力
	float HP = 70;

	//ボスの体力上限値
	float MAXHP = 70;

	//死亡時のパーティクルの色
	XMFLOAT4 BossParticleColor = { 0.988f, 0.443f, 0.180f, 1.0f };

	//ダメージ時の色
	XMFLOAT4 DamegeColor = { 1, 0, 0, 1 };

	bool DamegeFlag = false;

	//ダメージタイマー
	int DamegeTimer = 0;
};


