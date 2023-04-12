#pragma once
#include "Object3d.h"

//自機クラスの前方宣言
class Player;

class BossBullet :
    public Object3d
{
public:
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<BossBullet>Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 PlayerPos);


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
	//プレイヤーのセッター
	void SetPlayer(Player* player) { player_ = player; }

	//getter
	//デスフラグのゲッター
	bool DeathGetter() const { return DeathFlag; }

	//弾速のゲッター
	const XMFLOAT3& GetSpeed() { return Speed; }


	//プレイヤー
	Player* player_ = nullptr;

	//弾の寿命
	static const int LifeTimer = 60 * 2;

	//速度
	XMFLOAT3 Speed = { 0.0f, 0.0f, 0.0f };

	//移動用ベクトル
	XMFLOAT3 Vec = { 0.0f, 0.0f, 0.0f };

	//弾のデスタイマー
	int DeathTimer = LifeTimer;

	//弾のデスフラグ
	bool DeathFlag = false;

	//自機狙い弾用
	float PointPos = 0;
	float Count = 50.0f;
};

