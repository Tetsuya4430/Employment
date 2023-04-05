#pragma once
#include "Object3d.h"
class EnemyBullet :
    public Object3d
{
public:
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<EnemyBullet>Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 PlayerPos);


	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos, XMFLOAT3 PlayerPos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(XMFLOAT3 PlayerPos, XMFLOAT3 EnemyPos);

	//setter
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }

	//getter
	//デスフラグのゲッター
	const bool& GetDeathFlag() { return DeathFlag; }

	//弾速のゲッター
	const XMFLOAT3& GetSpeed() { return Speed; }

private:
	//弾の寿命
	static const int LifeTimer = 60 * 2;

	//速度
	XMFLOAT3 Speed = { 0.0f, 0.0f, 0.0f };

	//移動用ベクトル
	XMFLOAT3 Vec = { 0.0f, 0.0f, 0.0f };

	//弾のデスタイマー
	int DeathTimer = LifeTimer;

	//自機狙い弾用
	float PointPos = 0;
	float Count = 80.0f;

	//弾のデスフラグ
	bool DeathFlag = false;
};

