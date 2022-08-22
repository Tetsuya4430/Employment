#include "GameScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
#include "FrameWork.h"
#include "Controller.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete(object1);
	delete(model1);
	delete(postEffect);
	delete(player);
	delete(P);
	delete(E);
}

void GameScene::Initialize()
{ 
	////スプライト共通テクスチャ読み込み
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/GamePlay.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(100, L"Resources/Image/Sana.png");

	//	スプライトの生成
	sprite = Sprite::Create(1, { 0, 0 }, false, false);

	//ポストエフェクトの生成
	postEffect = PostEffect::Create(100, { 0, 0 }, false, false);

	//OBJからモデルデータを読み込む
	model_1 = Model::LoadFromObj("triangle_mat");
	model_2 = Model::LoadFromObj("Box");
	model_Bullet = Model::LoadFromObj("Bullet");
	model_Enemy = Model::LoadFromObj("Enemy");

	

	//3Dオブジェクト生成
	player = Object3d::Create(model_2, camera);

	P = Player::Create(model_2, camera);

	E = Enemy::Create(model_Enemy, camera);

	
	

		//デバイスをセット
		Fbx3d::SetDevice(dxCommon->GetDev());
		//カメラセット
		Fbx3d::SetCamera(camera);
		//グラフィックスパイプラインを生成
		Fbx3d::CreateGraphicsPipeline();

		//モデルを指定してFBXファイルを読み込み
		model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

		//3dオブジェクト生成とモデルのセット
		object1 = new Fbx3d;
		object1->Initialize();
		object1->SetModel(model1);

		object1->SetPosition({ 100.0f, 0.0f, 0.0f });

		//カメラ注視点をセット
		camera->SetTarget({ 0, 20, 0 });

	//音声読み込みと再生
	Audio::GetInstance()->LoadWave("Alarm01.wav");


	object1->PlayAnimation();
}

void GameScene::Finalize()
{
	//スプライト解放
	delete sprite;

	//モデルの解放
	delete model_1;
	delete model_2;
}

void GameScene::Update()
{
	//---デバッグテキスト関係---//
	//X座標、Y座標を指定して表示
	DebugText::GetInstance()->Print("Debug Text", 0, 0);

	//X座標、Y座標、縮尺を指定して表示
	DebugText::GetInstance()->Print("Debug Text = 0", 0, 50, 2.0f);

	

	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	


	//3Dオブジェクトの更新


	player->Update();

	P->Update();

	E->Update();

	Attack();

	//弾更新
	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Update(P->position_);
	}

	

	/*if (bullets)
	{
		bullet->Update(P->position_);
	}*/

	//B->Update();

	//FBXオブジェクトの更新
	object1->Update();
	
	//スプライトの更新
	sprite->Update();


	//カメラの更新
	camera->Update();


	//Escキーでウィンドウを閉じる
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESCキーでウィンドウを閉じる
	{
		//endRequest_ = true;
		return;
	}
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	////スプライトの共通コマンド
	SpriteCommon::GetInstance()->PreDraw();
	postEffect->PreDraw();

	////スプライト描画
	sprite->Draw();

	//3Dオブジェクトの描画前処理
	Object3d::PreDraw();



	P->Draw();

	E->Draw();



	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Draw();
	}

	

	//FBXオブジェクトの描画
	object1->Draw(cmdList);

	//3Dオブジェクトの描画後処理
	Object3d::PostDraw();

}

void GameScene::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_R))
	{
		//弾を生成し初期化
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet = Bullet::Create(model_Bullet, camera, P->position_);
		
		//弾を登録
		bullets.push_back(std::move(newBullet));
	}
}


