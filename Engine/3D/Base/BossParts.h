#pragma once
#include "Object3d.h"
class BossParts :
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
	};

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<BossParts>Create(Model* model, Camera* camera, XMFLOAT3 pos);


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
	void SetHP(int hp) { this->HP = hp; }
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }
	void SetDeathEffect(bool deathEffect) { this->DeathEffect = deathEffect; }
	void SetFireTime(bool fireTime) { this->FireTime = fireTime; }
	void SetIntervalTime(int intervalTime) { this->IntervalTime = intervalTime; }

	//getter
	const float& GetHP() { return HP; }
	const bool& GetDeathFlag() { return DeathFlag; }
	const bool& GetDeathEffect() { return DeathEffect; }
	const int& GetIntervalTime() { return IntervalTime; }
	const int& GetLastIntervalTime() { return LastIntervalTime; }
	const int& GetFireTime() { return FireTime; }
	const float& GetMAXHP() { return MAXHP; }
	const XMFLOAT4& GetBossParticleColor() { return BossParticleColor; }

private:
	//速度
	float Speed = 0.5f;

	//フェーズ
	Phase phase_ = Phase::Approach;

	bool DeathFlag = false;

	bool DeathEffect = false;

	int IntervalTime = 45;

	int LastIntervalTime = 10;

	float Shake = 0.5f;

	int FireTime = 0;

	int DownTimer = 0;

	//ボスの体力
	float HP = 10;

	//ボスの体力上限値
	float MAXHP = 10;

	//死亡時のパーティクルの色
	XMFLOAT4 BossParticleColor = { 0.988f, 0.443f, 0.180f, 1.0f };
};

