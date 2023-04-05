#pragma once
#include "Object3d.h"
class DefenceObject :
    public Object3d
{
public:
	//行動フェーズ
	enum class Phase
	{
		Approach,	//接近
	};

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static std::unique_ptr<DefenceObject>Create(Model* model, Camera* camera, XMFLOAT3 pos);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos);


	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(float speed);


	//setter
	void SetDeathFlag(bool deathFlag) { this->DeathFlag = deathFlag; }

	//getter
	const bool& GetDeathFlag() { return DeathFlag; }

private: // メンバ変数
	//速度
	float Speed = 0.4f;

	//フェーズ
	Phase phase_ = Phase::Approach;

	//デスフラグ
	bool DeathFlag = false;
};

