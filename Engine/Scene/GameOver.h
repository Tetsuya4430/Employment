#pragma once

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Audio.h"

/// <summary>
/// タイトルシーン
/// </summary>
class GameOver : public BaseScene
{
public:
	//初期化
	void Initialize(/*DirectXCommon* dxCommon*/) override;

	//終了処理
	void Finalize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;


private:
	DirectXCommon* dxCommon = nullptr;

	//スプライト
	std::unique_ptr <Sprite> sprite;
	std::unique_ptr <Sprite> LoadBG;


	Audio* audio = Audio::GetInstance();

	//ロードフラグ
	bool LoadFlag = false;
};

