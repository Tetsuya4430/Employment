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
	};

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static Boss* Create(Model* model, Camera* camera, XMFLOAT3 pos);


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
	void SetFireTime(bool fireTime) { this->FireTime = fireTime; }

	//getter
	const int& GetHP() { return HP; }
	const bool& GetDeathFlag() { return DeathFlag; }
	const int& GetIntervalTime() { return IntervalTime; }
	const int& GetFireTime() { return FireTime; }

private:
	//速度
	float Speed = 0.5f;

	//フェーズ
	Phase phase_ = Phase::Approach;

	int DeathFlag = false;

	int IntervalTime = 70;

	int FireTime = 0;

	//ボスの体力
	int HP = 15;
};


