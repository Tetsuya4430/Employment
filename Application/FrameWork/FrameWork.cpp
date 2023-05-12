#include "FrameWork.h"
#include "imgui.h"

void FrameWork::Run()
{
	//ゲームの初期化
	Initialize();

	while (true)	//ゲームループ
	{
		//毎フレーム更新
		Update();

		//終了リクエストが来たら抜ける
		if (IsEndRequest())
		{
			break;
		}
		//描画
		Draw();
	}
	//ゲームの終了
	Finalize();
}

void FrameWork::Initialize()
{
	//WindowsAPIの初期化
	winApp = WinApp::GetInstance();
	winApp->Initialize();

	//DirectXの初期化
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	//スプライト共通部分の初期化
	spriteCommon = SpriteCommon::GetInstance();
 	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	//デバッグテキスト
	debugText = DebugText::GetInstance();

	//デバッグテキスト用のテクスチャ番号を指定
	const int debugTextTexNumber = 0;
	//デバッグテキスト用のテクスチャ読み込み
	spriteCommon->SpriteCommonLoadTexture(debugTextTexNumber, L"Resources/Font/debugfont.png");
	//デバッグテキスト初期化
	debugText->Initialize(debugTextTexNumber);

	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);

	//オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	//ImGuiの初期化
	ImGui = ImGuiManager::GetInstance();
	ImGui->Initialize(winApp, dxCommon);

	//3Dオブジェクトの静的初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//プレイヤーの静的初期化
	Player::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	
	//レティクルの静的初期化
	//Reticle::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//敵の静的初期化
	Enemy::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//弾オブジェクトの静的初期化
	Bullet::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	//敵の弾オブジェクトの静的初期化
	EnemyBullet::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//ステージ1のボスオブジェクトの静的初期化
	Boss::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//ボスのパーツの静的初期化
	BossParts::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//ボス弾
	BossBullet::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//FBXローダーの初期化処理
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	//パーティクル初期化
	Particle::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	DefenceObject::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
}

void FrameWork::Finalize()
{
	//デバッグテキスト解放
	DebugText::GetInstance()->Finalize();
	
	//シーンマネージャーの解放
	SceneManager::GetInstance()->Finalize();

	//FBXローダーの解放
	FbxLoader::GetInstance()->Finalize();

	//オーディオ解放
	audio->Finalize();


	//ImGuiの解放
	ImGui->Finalize();

	//DirectX解放
	//delete dxCommon;

	//WindowsのAPIの終了処理
	winApp->Finalie();
}

void FrameWork::Update()
{
	//Windowsのメッセージ処理
	if (winApp->ProcessMessage())
	{
		//ゲームループを抜ける
		endRequest_ = true;
		return;
	}

	//入力の更新
	input->Update();

	//シーンの更新
	SceneManager::GetInstance()->Update();
}

void FrameWork::Draw()
{
	//描画前処理
	dxCommon->PreDraw();
	//ImGuiの更新
	ImGui->GetInstance()->ImGuiManager::Begin();
	SceneManager::GetInstance()->Draw();

	//デバッグテキスト描画
	debugText->DrawAll();
	//ImGui::Text("Hello, world %d", 123);

	//4,描画コマンドここまで
	//ImGuiの描画
	ImGui->GetInstance()->ImGuiManager::End();
	ImGui->GetInstance()->ImGuiManager::Draw();
	dxCommon->PostDraw();
}
