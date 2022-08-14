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

	////オブジェクトマネージャー生成
	//objectManager_1 = std::make_unique<ObjectManager>();
	//objectManager_2 = std::make_unique<ObjectManager>();
	//objectManager_3 = std::make_unique<ObjectManager>();
	//objectManager_4 = std::make_unique<ObjectManager>();

	////オブジェクトマネージャー生成->3Dオブジェクト生成->オブジェクトの参照カウントを増加
	//std::shared_ptr<Object3d> object_1 = Object3d::Create(model_1, camera);
	//std::weak_ptr<Object3d> ObjectWp_1 = objectManager_1->AddObject(object_1);
	//Empty = ObjectWp_1.lock();

	//std::shared_ptr<Object3d> object_2 = Object3d::Create(model_Bullet, camera);
	//std::weak_ptr<Object3d> ObjectWp_2 = objectManager_2->AddObject(object_2);
	//BulletObj = ObjectWp_2.lock();

	//std::shared_ptr<Object3d> object_3 = Object3d::Create(model_2, camera);
	//std::weak_ptr<Object3d> ObjectWp_3 = objectManager_3->AddObject(object_3);
	//player = ObjectWp_3.lock();

	//std::shared_ptr<Object3d> object_4 = Object3d::Create(model_Enemy, camera);
	//std::weak_ptr<Object3d> ObjectWp_4 = objectManager_4->AddObject(object_4);
	//Enemy = ObjectWp_4.lock();

	//3Dオブジェクト生成
	player = Object3d::Create(model_2, camera);

	P = Player::Create(model_2, camera);

	B = Bullet::Create(model_Bullet, camera);

	//3Dオブジェクトの位置と拡大率を指定
	
		/*Empty->SetScale({ 20.0f, 20.0f, 20.0f });

		BulletObj->SetPosition({ 0, 0, 0 });
		BulletObj->SetScale({ 0.5f, 0.5f, 0.5f });
	
		player->SetPosition({ 0, 0, 0 });
		player->SetScale({ 1.0f, 1.0f, 1.0f });

		Enemy->SetPosition({ 0, 30, 20 });
		Enemy->SetScale({ 1.0f, 1.0f, 1.0f });*/
	
		//プレイヤーの初期化
		//player = player::Create(model_1, camera);

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

	////プレイヤーの座標を取得
	//DirectX::XMFLOAT3 PlayerPos = player->GetPosition();

	////プレイヤーの座標を取得
	//DirectX::XMFLOAT3 EnemyPos = Enemy->GetPosition();

	//DirectX::XMFLOAT3 EnemyPos_2 = {EnemyPos.x + 10.0f, EnemyPos.y - 10.0f , EnemyPos.z + 10.0f };


	////弾のサイズを取得
	//DirectX::XMFLOAT3 BulletPos = BulletObj->GetPosition();

	//DirectX::XMFLOAT3 BulletPos_2 = { BulletPos.x + 5.0f, BulletPos.y - 5.0f , BulletPos.z + 5.0f };

	////プレイヤーのローテーション取得
	//DirectX::XMFLOAT3 PlayerRot = player->GetRotation();

	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//if (Input::GetInstance()->TriggerKey(DIK_SPACE) && BulletFlag == 0)
	//{
	//	BulletFlag = 1;
	//}

	//if (Input::GetInstance()->TriggerKey(DIK_A))
	//{
	//	Audio::GetInstance()->PlayWave("Alarm01.wav");
	//}

	////座標操作
	//if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))
	//{
	//		if (Input::GetInstance()->PushKey(DIK_DOWN))
	//		{
	//			MoveFlag = 1;
	//			PlayerPos.y -= MoveVec;
	//			player->SetPosition(PlayerPos);
	//		}

	//		if (Input::GetInstance()->PushKey(DIK_UP))
	//		{
	//			MoveFlag = 1;
	//			PlayerPos.y += MoveVec;
	//			player->SetPosition(PlayerPos);
	//		}

	//		if (Input::GetInstance()->PushKey(DIK_LEFT))
	//		{
	//			MoveFlag = 1;
	//			PlayerPos.x -= MoveVec;
	//			player->SetPosition(PlayerPos);
	//			if (PlayerRot.z <= 10.0f)
	//			{
	//				PlayerRot.z += 1.0f;
	//				player->SetRotation(PlayerRot);
	//			}
	//		}

	//		if (Input::GetInstance()->PushKey(DIK_RIGHT))
	//		{
	//			MoveFlag = 1;
	//			PlayerPos.x += MoveVec;
	//			player->SetPosition(PlayerPos);
	//			if (PlayerRot.z >= -10.0f)
	//			{
	//				PlayerRot.z -= 1.0f;
	//				player->SetRotation(PlayerRot);
	//			}
	//		}
	//}
	//else
	//{
	//	MoveFlag = 0;
	//}

	////プレイヤーが傾いている且つ移動していない時は角度を戻す
	//if (MoveFlag  == 0)
	//{
	//	if (PlayerRot.z > 0)
	//	{
	//		PlayerRot.z -= 1.0f;
	//		player->SetRotation(PlayerRot);
	//	}
	//}

	//if (MoveFlag == 0)
	//{
	//	if (PlayerRot.z < 0)
	//	{
	//		PlayerRot.z += 1.0f;
	//		player->SetRotation(PlayerRot);
	//	}
	//}

	////コントローラー移動



	//if (BulletFlag == 0)
	//{
	//	BulletObj->SetPosition(PlayerPos);
	//}


	//if (BulletFlag == 1)
	//{
	//	BulletPos.z += 1.0f;

	//	BulletObj->SetPosition(BulletPos);
	//}

	//if (BulletPos.z >= 30)
	//{
	//	BulletFlag = 0;
	//	BulletPos.z = 0;
	//	BulletObj->SetPosition(BulletPos);
	//}

	////弾と敵の当たり判定
	//if (EnemyPos.x < BulletPos.x && EnemyPos_2.x > BulletPos.x)
	//{
	//	if (EnemyPos.y > BulletPos.y && EnemyPos_2.y < BulletPos.y)
	//	{
	//		if (EnemyPos.z)
	//		{

	//		}
	//	}
	//}


	//3Dオブジェクトの更新
	/*objectManager_1->Update();
	objectManager_2->Update();
	objectManager_3->Update();
	objectManager_4->Update();*/

	player->Update();

	P->Update();

	B->Update();

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

	//敵の描画
	//objectManager_1->Draw();

	//弾の描画
	//if (BulletFlag == 1)
	//{
	//	objectManager_2->Draw();
	//}

	////プレイヤーの描画
	//objectManager_3->Draw();

	////プレイヤーの描画
	//objectManager_4->Draw();

	//player->Draw();

	//P->Draw();
	B->Draw();

	//FBXオブジェクトの描画
	object1->Draw(cmdList);

	//3Dオブジェクトの描画後処理
	Object3d::PostDraw();

}


