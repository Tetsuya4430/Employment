#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "Controller.h"

void TitleScene::Initialize(/*DirectXCommon* dxCommon*/)
{
	////スプライト共通テクスチャ読み込み
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/Title.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(2, L"Resources/Image/GameStart.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(3, L"Resources/Image/LoadBG.png");

	//	スプライトの生成
	sprite = Sprite::Create(1, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	start = Sprite::Create(2, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	LoadBG = Sprite::Create(3, { 1, 1, 1, 1 }, { 0, 0 }, false, false);

	LoadFlag = false;
	LoadBG->color_.w = 1.0f;

	Audio::GetInstance()->LoadWave("Title.wav");

	Audio::GetInstance()->LoadWave("Decision.wav");

	Audio::GetInstance()->PlayWave("Title.wav", Title_Vol, true);


	InitInput();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		Audio::GetInstance()->PlayWave("Decision.wav", Decision_Vol, true);
		LoadFlag = true;
	}

	if (IsButtonDown(ButtonKind::Button_A))
	{
		Audio::GetInstance()->PlayWave("Decision.wav", Decision_Vol, true);
		LoadFlag = true;
	}

	//コントローラーの押下情報更新
		UpdateInput();

	if (LoadBG->color_.w >= 0.0f && LoadFlag == false)
	{
		LoadBG->color_.w -= 0.02f;
	}

	if (LoadFlag == true)
	{
		LoadBG->color_.w += 0.02f;
	}


	if (LoadBG->color_.w >= 1.0f)
	{
		Audio::GetInstance()->StopWave("Title.wav");
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	
	//スプライトの更新
	sprite->Update();

	
		LoadBG->Update();



	//Escキーでウィンドウを閉じる
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESCキーでウィンドウを閉じる
	{
		//endRequest_ = true;
		return;
	}
}

void TitleScene::Draw()
{
	////スプライトの共通コマンド
	SpriteCommon::GetInstance()->PreDraw();

	////スプライト描画
	sprite->Draw();


	
		LoadBG->Draw();
	

	//start->Draw();

}
