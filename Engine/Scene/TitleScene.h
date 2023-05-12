#pragma once

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene
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
	std::unique_ptr <Sprite>sprite;
	std::unique_ptr <Sprite>start;
	std::unique_ptr <Sprite>LoadBG;

	Audio* audio = Audio::GetInstance();

	//ロードフラグ
	bool LoadFlag = false;

	//音量
	float Decision_Vol = 0.3f;

	float Title_Vol = 0.1f;

	//後で消す
	int Timer = 0;
};

