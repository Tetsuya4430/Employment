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

#include <iomanip>

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
}

void GameScene::Initialize()
{
	////スプライト共通テクスチャ読み込み
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/BackGround.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(2, L"Resources/Image/Reticle.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(3, L"Resources/Image/HP_5.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(4, L"Resources/Image/HP_4.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(5, L"Resources/Image/HP_3.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(6, L"Resources/Image/HP_2.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(7, L"Resources/Image/HP_1.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(8, L"Resources/Image/HP_0.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(25, L"Resources/Image/ExpBar/ExpUI_0.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(26, L"Resources/Image/ExpBar/ExpUI_1.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(27, L"Resources/Image/ExpBar/ExpUI_2.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(28, L"Resources/Image/ExpBar/ExpUI_3.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(29, L"Resources/Image/ExpBar/ExpUI_4.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(30, L"Resources/Image/ExpBar/ExpUI_5.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(31, L"Resources/Image/ExpBar/Level_1.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(32, L"Resources/Image/ExpBar/Level_2.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(33, L"Resources/Image/ExpBar/Level_3.png");

	//	スプライトの生成
	sprite = Sprite::Create(1, { 0, 0 }, false, false);
	UI = Sprite::Create(2, { 0.5, 0.5 }, false, false);
	HP_0 = Sprite::Create(8, { 0, 0 }, false, false);
	HP_1 = Sprite::Create(7, { 0, 0 }, false, false);
	HP_2 = Sprite::Create(6, { 0, 0 }, false, false);
	HP_3 = Sprite::Create(5, { 0, 0 }, false, false);
	HP_4 = Sprite::Create(4, { 0, 0 }, false, false);
	HP_5 = Sprite::Create(3, { 0, 0 }, false, false);

	ExpBar_0 = Sprite::Create(25, { 0, 0 }, false, false);
	ExpBar_1 = Sprite::Create(26, { 0, 0 }, false, false);
	ExpBar_2 = Sprite::Create(27, { 0, 0 }, false, false);
	ExpBar_3 = Sprite::Create(28, { 0, 0 }, false, false);
	ExpBar_4 = Sprite::Create(29, { 0, 0 }, false, false);
	ExpBar_5 = Sprite::Create(30, { 0, 0 }, false, false);
	Level_1 = Sprite::Create(31, { 0, 0 }, false, false);
	Level_2 = Sprite::Create(32, { 0, 0 }, false, false);
	Level_3 = Sprite::Create(33, { 0, 0 }, false, false);

	

	LoadSprite();
	//ポストエフェクトの生成
	//postEffect = PostEffect::Create(100, { 0, 0 }, false, false);

	//OBJからモデルデータを読み込む
	//model_1 = Model::LoadFromObj("triangle_mat");
	model_2 = Model::LoadFromObj("Player");
	model_Bullet = Model::LoadFromObj("Bullet");
	model_EnemyBullet = Model::LoadFromObj("EnemyBullet");
	model_Enemy = Model::LoadFromObj("Enemy_1");
	model_Boss = Model::LoadFromObj("Boss");
	model_sphere = Model::LoadFromObj("CelestialSphere");
	model_reticle = Model::LoadFromObj("Reticle");


	

	//敵の初期化
	LoadEnemyPopData();
	EnemyInit();


	//3Dオブジェクト生成
	P = Player::Create(model_2, camera);

	//天球生成
	CelestialSphere = Object3d::Create(model_sphere, camera);
	CelestialSphere->SetScale({100, 100, 100});

	//レティクル生成
	Reticle = Reticle::Create(model_reticle, camera);
	Reticle->SetScale({ 2, 2, 2 });



		//カメラ注視点をセット
		camera->SetTarget({ 0, 20, 0 });


	/*object1->PlayAnimation();*/

	//演出タイマー初期化
	WaitTimer = 0;
	BossTimer = 128;
}

void GameScene::Finalize()
{
	//スプライト解放
	delete sprite;
	delete UI;
	delete HP_0;
	delete HP_1;
	delete HP_2;
	delete HP_3;
	delete HP_4;
	delete HP_5;

	//モデルの解放
	delete model_1;
	delete model_2;
}

void GameScene::Update()
{
	//---デバッグテキスト関係---//

	//マウス座標を2Dレティクルのスプライトに代入
	UI->position_.x = Reticle->MousePosition.x;
	UI->position_.y = Reticle->MousePosition.y;

	

	//シーン遷移
	//敵のHPが0になったらゲームクリア
	if (Boss)
	{
		if (Boss->HP <= 0 && BossTimer <= 0)
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

	//Reticle->Update();

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
					P->EXP += 1;
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

	//レティクル更新
	Reticle->Update(P->position_);

	//FBXオブジェクトの更新
	//object1->Update();
	
	/*UI->position_.x = MousePosition.x;
	UI->position_.y = MousePosition.y;*/

	//スプライトの更新
	sprite->Update();
	UI->Update();
	HP_0->Update();
	HP_1->Update();
	HP_2->Update();
	HP_3->Update();
	HP_4->Update();
	HP_5->Update();
	ExpBar_0->Update();
	ExpBar_1->Update();
	ExpBar_2->Update();
	ExpBar_3->Update();
	ExpBar_4->Update();
	ExpBar_5->Update();

	UpdateSprite();


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

	//////スプライトの共通コマンド
	//SpriteCommon::GetInstance()->PreDraw();
	postEffect->PreDraw();

	////スプライト描画



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

		//ボス登場タイマーのデクリメント
		if (BossTimer > 0)
		{
			BossTimer--;
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

	//レティクル描画
	Reticle->Draw();

	//FBXオブジェクトの描画
	//object1->Draw(cmdList);


	//3Dオブジェクトの描画後処理
	Object3d::PostDraw();

	////スプライトの共通コマンド
	SpriteCommon::GetInstance()->PreDraw();
	
	UI->Draw();

	if (P->HP == 5)
	{
		HP_5->Draw();
	}

	if (P->HP == 4)
	{
		HP_4->Draw();
	}

	if (P->HP == 3)
	{
		HP_3->Draw();
	}

	if (P->HP == 2)
	{
		HP_2->Draw();
	}

	if (P->HP == 1)
	{
		HP_1->Draw();
	}

	if (P->HP == 0)
	{
		HP_0->Draw();
	}



	DrawSprite();

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
	
}

void GameScene::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		//弾を生成し初期化
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet = Bullet::Create(model_Bullet, camera, P->position_, Reticle->position_);
		
		//弾を登録
		bullets.push_back(std::move(newBullet));
	}
}

void GameScene::EnemyAttack(XMFLOAT3 EnemyPos)
{
	
		//弾を生成し初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet = EnemyBullet::Create(model_EnemyBullet, camera, EnemyPos, P->position_);

		//弾を登録
		enemybullets.push_back(std::move(newBullet));

		//enemybullets
}

void GameScene::BossAttack(XMFLOAT3 BossPos)
{
	//弾を生成し初期化
	std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
	newBullet = BossBullet::Create(model_EnemyBullet, camera, BossPos, P);

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

void GameScene::LoadSprite()
{
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(9, L"Resources/Image/BossHP/BossHP_0.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(10, L"Resources/Image/BossHP/BossHP_1.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(11, L"Resources/Image/BossHP/BossHP_2.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(12, L"Resources/Image/BossHP/BossHP_3.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(13, L"Resources/Image/BossHP/BossHP_4.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(14, L"Resources/Image/BossHP/BossHP_5.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(15, L"Resources/Image/BossHP/BossHP_6.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(16, L"Resources/Image/BossHP/BossHP_7.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(17, L"Resources/Image/BossHP/BossHP_8.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(18, L"Resources/Image/BossHP/BossHP_9.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(19, L"Resources/Image/BossHP/BossHP_10.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(20, L"Resources/Image/BossHP/BossHP_11.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(21, L"Resources/Image/BossHP/BossHP_12.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(22, L"Resources/Image/BossHP/BossHP_13.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(23, L"Resources/Image/BossHP/BossHP_14.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(24, L"Resources/Image/BossHP/BossHP_15.png");


	BossHP_0 = Sprite::Create(9, { 0, 0 }, false, false);
	BossHP_1 = Sprite::Create(10, { 0, 0 }, false, false);
	BossHP_2 = Sprite::Create(11, { 0, 0 }, false, false);
	BossHP_3 = Sprite::Create(12, { 0, 0 }, false, false);
	BossHP_4 = Sprite::Create(13, { 0, 0 }, false, false);
	BossHP_5 = Sprite::Create(14, { 0, 0 }, false, false);
	BossHP_6 = Sprite::Create(15, { 0, 0 }, false, false);
	BossHP_7 = Sprite::Create(16, { 0, 0 }, false, false);
	BossHP_8 = Sprite::Create(17, { 0, 0 }, false, false);
	BossHP_9 = Sprite::Create(18, { 0, 0 }, false, false);
	BossHP_10 = Sprite::Create(19, { 0, 0 }, false, false);
	BossHP_11 = Sprite::Create(20, { 0, 0 }, false, false);
	BossHP_12 = Sprite::Create(21, { 0, 0 }, false, false);
	BossHP_13 = Sprite::Create(22, { 0, 0 }, false, false);
	BossHP_14 = Sprite::Create(23, { 0, 0 }, false, false);
	BossHP_15 = Sprite::Create(24, { 0, 0 }, false, false);
}

void GameScene::UpdateSprite()
{
	if (Boss)
	{
		BossHP_0->Update();
		BossHP_1->Update();
		BossHP_2->Update();
		BossHP_3->Update();
		BossHP_4->Update();
		BossHP_5->Update();
		BossHP_6->Update();
		BossHP_7->Update();
		BossHP_8->Update();
		BossHP_9->Update();
		BossHP_10->Update();
		BossHP_11->Update();
		BossHP_12->Update();
		BossHP_13->Update();
		BossHP_14->Update();
		BossHP_15->Update();
	}
}

void GameScene::DrawSprite()
{
	//体力演出中の描画

	if (BossTimer < 128 && BossTimer > 120)
	{
		BossHP_0->Draw();
	}

	if (BossTimer <= 120 && BossTimer > 112)
	{
		BossHP_1->Draw();
	}

	if (BossTimer <= 112 && BossTimer > 104)
	{
		BossHP_2->Draw();
	}

	if (BossTimer <= 104 && BossTimer > 96)
	{
		BossHP_3->Draw();
	}

	if (BossTimer <= 96 && BossTimer > 88)
	{
		BossHP_4->Draw();
	}

	if (BossTimer <= 88 && BossTimer > 80)
	{
		BossHP_5->Draw();
	}

	if (BossTimer <= 80 && BossTimer > 72)
	{
		BossHP_6->Draw();
	}

	if (BossTimer <= 72 && BossTimer > 64)
	{
		BossHP_7->Draw();
	}

	if (BossTimer <= 64 && BossTimer > 56)
	{
		BossHP_8->Draw();
	}

	if (BossTimer <= 56 && BossTimer > 48)
	{
		BossHP_9->Draw();
	}

	if (BossTimer <= 48 && BossTimer > 40)
	{
		BossHP_10->Draw();
	}

	if (BossTimer <= 40 && BossTimer > 32)
	{
		BossHP_11->Draw();
	}

	if (BossTimer <= 32 && BossTimer > 26)
	{
		BossHP_12->Draw();
	}

	if (BossTimer <= 26 && BossTimer > 18)
	{
		BossHP_13->Draw();
	}

	if (BossTimer <= 18 && BossTimer > 10)
	{
		BossHP_14->Draw();
	}

	if (BossTimer <= 10 && BossTimer > 0)
	{
		BossHP_15->Draw();
	}

	//ボス体力演出後の描画
	if (Boss && BossTimer <= 0)
	{
		if (Boss->HP == 15)
		{
			BossHP_15->Draw();
		}

		if (Boss->HP == 14)
		{
			BossHP_14->Draw();
		}

		if (Boss->HP == 13)
		{
			BossHP_13->Draw();
		}

		if (Boss->HP == 12)
		{
			BossHP_12->Draw();
		}

		if (Boss->HP == 11)
		{
			BossHP_11->Draw();
		}

		if (Boss->HP == 10)
		{
			BossHP_10->Draw();
		}

		if (Boss->HP == 9)
		{
			BossHP_9->Draw();
		}

		if (Boss->HP == 8)
		{
			BossHP_8->Draw();
		}

		if (Boss->HP == 7)
		{
			BossHP_7->Draw();
		}

		if (Boss->HP == 6)
		{
			BossHP_6->Draw();
		}

		if (Boss->HP == 5)
		{
			BossHP_5->Draw();
		}

		if (Boss->HP == 4)
		{
			BossHP_4->Draw();
		}

		if (Boss->HP == 3)
		{
			BossHP_3->Draw();
		}

		if (Boss->HP == 2)
		{
			BossHP_2->Draw();
		}

		if (Boss->HP == 1)
		{
			BossHP_1->Draw();
		}

		if (Boss->HP == 0)
		{
			BossHP_0->Draw();
		}
	}
	
	//経験値UIの描画
	if (P->EXP == 0)
	{
		ExpBar_0->Draw();
	}

	else if (P->EXP == 1)
	{
		ExpBar_1->Draw();
	}

	else if (P->EXP == 2)
	{
		ExpBar_2->Draw();
	}

	else if (P->EXP == 3)
	{
		ExpBar_3->Draw();
	}

	else if (P->EXP == 4)
	{
		ExpBar_4->Draw();
	}

	else if (P->EXP == 5)
	{
		ExpBar_5->Draw();
	}

	//レベル
	if (P->Level == 1)
	{
		Level_1->Draw();
	}

	else if (P->Level == 2)
	{
		Level_2->Draw();
	}

	else if (P->Level == 3)
	{
		Level_3->Draw();
	}

}



