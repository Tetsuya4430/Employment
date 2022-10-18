#include "GameClear.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"

void GameClear::Initialize(/*DirectXCommon* dxCommon*/)
{
	////スプライト共通テクスチャ読み込み
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/Iroha.jpg");

	//	スプライトの生成
	sprite = Sprite::Create(1, { 0, 0 }, false, false);
}

void GameClear::Finalize()
{
	//スプライト解放
	delete sprite;


}

void GameClear::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//スプライトの更新
	sprite->Update();


	//Escキーでウィンドウを閉じる
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESCキーでウィンドウを閉じる
	{
		//endRequest_ = true;
		return;
	}
}

void GameClear::Draw()
{
	////スプライトの共通コマンド
	SpriteCommon::GetInstance()->PreDraw();

	////スプライト描画
	sprite->Draw();

}
