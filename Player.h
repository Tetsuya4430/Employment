#include "DirectXCommon.h"
#include "Fbx3d.h"

#pragma once
class Player
{
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize();

	/// <summary>
	/// XV
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();

private:
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;
	Camera* camera = nullptr;
};

