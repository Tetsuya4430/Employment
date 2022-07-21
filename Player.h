#include "DirectXCommon.h"
#include "Fbx3d.h"

#pragma once
class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;
	Camera* camera = nullptr;
};

