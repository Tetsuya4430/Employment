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

using namespace std;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//delete(object1);
	delete(model1);
	delete(postEffect);
	delete(P);
	delete(Boss);
	delete(particle);
}

void GameScene::Initialize()
{ 
	////スプライト共通テクスチャ読み込み
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/BackGround.png");
	//SpriteCommon::GetInstance()->SpriteCommonLoadTexture(100, L"Resources/Image/Sana.png");

	//	スプライトの生成
	sprite = Sprite::Create(1, { 0, 0 }, false, false);

	//ポストエフェクトの生成
	//postEffect = PostEffect::Create(100, { 0, 0 }, false, false);

	//OBJからモデルデータを読み込む
	//model_1 = Model::LoadFromObj("triangle_mat");
	model_2 = Model::LoadFromObj("Player");
	model_Bullet = Model::LoadFromObj("Bullet");
	model_Enemy = Model::LoadFromObj("Enemy");
	model_Boss = Model::LoadFromObj("Boss");
	model_sphere = Model::LoadFromObj("CelestialSphere");


	

	//敵の初期化
	LoadEnemyPopData();
	EnemyInit();


	//3Dオブジェクト生成
	P = Player::Create(model_2, camera);

	CelestialSphere = Object3d::Create(model_sphere, camera);
	CelestialSphere->SetScale({100, 100, 100});

	//part = Part::Create(model_1, camera);

	//敵の弾に自キャラにアドレスを渡す
	//enemybullets

		////デバイスをセット
		//Fbx3d::SetDevice(dxCommon->GetDev());
		////カメラセット
		//Fbx3d::SetCamera(camera);
		////グラフィックスパイプラインを生成
		//Fbx3d::CreateGraphicsPipeline();

		//モデルを指定してFBXファイルを読み込み
	//	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

		//3dオブジェクト生成とモデルのセット
	/*	object1 = new Fbx3d;
		object1->Initialize();
		object1->SetModel(model1);

		object1->SetPosition({ 100.0f, 0.0f, 0.0f });*/

		////パーティクルの生成
		//particle = Particle::Create(camera);
		//particle->Update();

		

		//カメラ注視点をセット
		camera->SetTarget({ 0, 20, 0 });

	//音声読み込みと再生
	Audio::GetInstance()->LoadWave("Alarm01.wav");


	/*object1->PlayAnimation();*/

	//演出タイマー初期化
	WaitTimer = 0;
}

void GameScene::Finalize()
{
	//スプライト解放
	delete sprite;

	//モデルの解放
	//delete model_1;
	delete model_2;
}

void GameScene::Update()
{
	//---デバッグテキスト関係---//
	////X座標、Y座標を指定して表示
	//DebugText::GetInstance()->Print("Debug Text", 0, 0);

	////X座標、Y座標、縮尺を指定して表示
	//DebugText::GetInstance()->Print("Debug Text = 0", 0, 50, 2.0f);

	
	//シーン遷移
	//敵のHPが0になったらゲームクリア
	if (Boss)
	{
		if (Boss->HP <= 0)
		{
			if (WaitTimer < 60)
			{
				WaitTimer += 1;
			}

			if (WaitTimer >= 60)
			{
				WaitTimer = 0;
				SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
			}
		}
	}
		//プレイヤーのHPが0になったらゲームオーバー
	if (P->HP <= 0)
	{
		if (WaitTimer < 60)
		{
			WaitTimer += 1;
		}

		if (WaitTimer >= 60)
		{
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
	}
	

	//3Dオブジェクトの更新


	//player->Update();

	//プレイヤーの更新
	P->Update();

	//プレイヤーの攻撃関数
	Attack();

	if (Boss)
	{
		if (Boss->DeathFlag == false)
		{
			//ボスの更新
			Boss->Update();
		}
	}


	UpdateEnemyPopCommands();

	//敵の更新
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (enemy->DeathFlag == false)
		{
			//EnemyUpdate(enemy->position);
			enemy->Update();


			enemy->FireTime--;

			if (enemy->FireTime <= 0)
			{
				EnemyAttack(enemy->position);

				//発射タイマーを初期化
				enemy->FireTime = enemy->IntervalTime;
			}
		}
		
	}

	//ボスの更新
	if (Boss)
	{
		if (Boss->DeathFlag == false)
		{
			Boss->Update();


			Boss->FireTime--;

			if (Boss->FireTime <= 0)
			{
				EnemyAttack(Boss->position);

				//発射タイマーを初期化
				Boss->FireTime = Boss->IntervalTime;
			}

		}
	}


	//弾更新
	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Update(P->position_);
	}

	//敵の弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
	{
		//仮として引数をプレイヤーの座標にしている
		bullet->Update(P->position_, P->position_);
	}

	//ボスの弾更新
	for (std::unique_ptr<BossBullet>& bullet : bossbullets)
	{
		//仮として引数をプレイヤーの座標にしている
		bullet->Update(P->position_, P->position_);
	}

	//デスフラグの立った弾の削除
	//プレイヤー弾
	bullets.remove_if([](std::unique_ptr<Bullet>&bullet)
	{
		return bullet->DeathGetter();
	});

	//敵弾
	enemybullets.remove_if([](std::unique_ptr<EnemyBullet>& Enemybullet)
	{
			return Enemybullet->DeathGetter();
	});

	//ボス弾
	bossbullets.remove_if([](std::unique_ptr<BossBullet>& Bossbullet)
		{
			return Bossbullet->DeathGetter();
		});


	//当たり判定

	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		for (std::unique_ptr<Enemy>& enemy : enemys)
		{
			//当たり判定確認
			if (enemy->DeathFlag == false)
			{
				if (CheckCollision(bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
				{
					bullet->DeathFlag = true;
					enemy->DeathFlag = true;
				}
			}
		}
	}

	
	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
	{
		//自機と敵の弾当たり判定確認
		if (CheckCollision(P->GetPosition(), bullet->GetPosition(), 2.0f, 2.0f) == true)
		{
			P->HP -= 1;
			bullet->DeathFlag = true;
		}
	}

	//プレイヤーの弾とボスの当たり判定
	if (Boss)
	{
		if (Boss->DeathFlag == false)
		{
			for (std::unique_ptr<Bullet>& bullet : bullets)
			{
				if (CheckCollision(bullet->GetPosition(), Boss->GetPosition(), 2.0f, 6.0f) == true)
				{
					Boss->HP -= 1;
					bullet->DeathFlag = true;
				}
			}
		}

		if (Boss->HP <= 0)
		{
			Boss->DeathFlag = true;
		}
	}

	//ボスの弾とプレイヤーの当たり判定
	if (Boss)
	{
		for (std::unique_ptr<BossBullet>& bullet : bossbullets)
		{
			//自機と敵の弾当たり判定確認
			if (CheckCollision(P->GetPosition(), bullet->GetPosition(), 2.0f, 2.0f) == true)
			{
				P->HP -= 1;
				bullet->DeathFlag = true;
			}
		}
	}

	//天球更新
	CelestialSphere->Update();

	//FBXオブジェクトの更新
	//object1->Update();
	
	//スプライトの更新
	sprite->Update();

	//パーティクルの更新
	//particle->Update();

	//part->Update();


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

	/*SpriteCommon::PostDraw();*/
	
	//3Dオブジェクトの描画前処理
	Object3d::PreDraw();

	P->Draw();

	if (Boss)
	{
		if (Boss->DeathFlag == false)
		{
			Boss->Draw();
		}
	}

	//敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (enemy->DeathFlag == false)
		{
			enemy->Draw();
		}
	}

	//プレイヤーの弾描画
	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Draw();
	}

	//敵の弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
	{
		bullet->Draw();
	}

	//ボスの弾描画
	for (std::unique_ptr<BossBullet>& bullet : bossbullets)
	{
		bullet->Draw();
	}

	//天球の描画
	CelestialSphere->Draw();

	//part->Draw();

	//FBXオブジェクトの描画
	//object1->Draw(cmdList);


	//3Dオブジェクトの描画後処理
	Object3d::PostDraw();

}

void GameScene::EnemyInit()
{
	//発射タイマーを初期化
	//EnemyBulletTimer = BulletInterval;

	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->FireTime = enemy->IntervalTime;
	}
}

void GameScene::BossInit()
{
	//発射タイマーを初期化
	//EnemyBulletTimer = BulletInterval;

		Boss->FireTime = Boss->IntervalTime;
}

void GameScene::EnemyUpdate(XMFLOAT3 enemyPos)
{
	EnemyBulletTimer--;
	

	//for (std::unique_ptr<Enemy>& enemy : enemys)
	//{
	//	if (EnemyBulletTimer <= 0 /*&& enemy->DeathFlag == 0*/)
	//	{
	//		EnemyAttack(/*enemy->position*/ enemyPos);

	//		//発射タイマーを初期化
	//		EnemyBulletTimer = BulletInterval;
	//	}
	//}	
}

void GameScene::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		//弾を生成し初期化
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet = Bullet::Create(model_Bullet, camera, P->position_);
		
		//弾を登録
		bullets.push_back(std::move(newBullet));
	}
}

void GameScene::EnemyAttack(XMFLOAT3 EnemyPos)
{
	
		//弾を生成し初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet = EnemyBullet::Create(model_Bullet, camera, EnemyPos, P);

		/*newBullet->position_B.x = EnemyPos.x;
		newBullet->position_B.y = EnemyPos.y;
		newBullet->position_B.z = EnemyPos.z;

		float xv = P->position_.x - newBullet->position_B.x;
		float yv = P->position_.y - newBullet->position_B.y;
		float zv = P->position_.z - newBullet->position_B.z;

		float v = sqrtf(xv * xv + yv * yv + zv * zv);

		newBullet->position_B.x = (xv / v) * 2;
		newBullet->position_B.y = (yv / v) * 2;
		newBullet->position_B.z = (zv / v) * 2;*/

		//弾を登録
		enemybullets.push_back(std::move(newBullet));

		//enemybullets
}

void GameScene::BossAttack(XMFLOAT3 BossPos)
{
	//弾を生成し初期化
	std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
	newBullet = BossBullet::Create(model_Bullet, camera, BossPos, P);

	bossbullets.push_back(std::move(newBullet));
}

void GameScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open(L"Resources/EnemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	//待機処理
	if (EnemyFlag)
	{
		EnemyTimer--;

		if (EnemyTimer <= 0)
		{
			//待機完了
			EnemyFlag = false;
		}
		return;
	}

	//1桁分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line))
	{
		//1桁分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;

		//,区切りでの行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメントアウト
		if (word.find("//") == 0)
		{
			//コメントを飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0)
		{
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());


			//取得したx,y,z座標を格納
			XMFLOAT3 EnemyPos = { x, y, z };

			//敵を発生させる
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy = Enemy::Create(model_Enemy, camera, EnemyPos);

			enemys.push_back(std::move(newEnemy));

		}

		//POPコマンド
		else if (word.find("BOSS_POP") == 0)
		{
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());


			//取得したx,y,z座標を格納
			XMFLOAT3 EnemyPos = { x, y, z };

			//敵を発生させる
			Boss = St1_Boss::Create(model_Boss, camera, EnemyPos);

		}

		//WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機時間
			EnemyFlag = true;
			EnemyTimer = waitTime;

			//コマンドループを抜ける
			break;
		}

	}
}

bool GameScene::CheckCollision(XMFLOAT3 Object1, XMFLOAT3 Object2, float R1, float R2)
{
	float Check = sqrtf((Object1.x - Object2.x) * (Object1.x - Object2.x) + (Object2.y - Object1.y) * (Object2.y - Object1.y) + (Object1.z - Object2.z) * (Object1.z - Object2.z));

	if (Check <= R1 - R2 || Check <= R2 - R1 || Check < R1 + R2 )
	{
		return true;
	}

	else
	{
		return false;
	}
}



