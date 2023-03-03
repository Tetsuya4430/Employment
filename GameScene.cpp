#include "GameScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
#include "FrameWork.h"
#include "Controller.h"

#include <DirectXMath.h>
#include <iomanip>

using namespace std;
using namespace DirectX;

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
	
	
	LoadSprite();
	//ポストエフェクトの生成
	//postEffect = PostEffect::Create(100, { 0, 0 }, false, false);

	//OBJからモデルデータを読み込む
	model_2 = Model::LoadFromObj("Player");
	model_Bullet = Model::LoadFromObj("Bullet");
	model_EnemyBullet = Model::LoadFromObj("EnemyBullet");
	model_Enemy = Model::LoadFromObj("Enemy_1");
	model_Boss = Model::LoadFromObj("Boss");
	model_sphere = Model::LoadFromObj("CelestialSphere");
	model_Moon = Model::LoadFromObj("Moon");
	model_Meteor = Model::LoadFromObj("Meteor");
	model_Station = Model::LoadFromObj("Station");
	//model_SpaceStation = Model::LoadFromObj("SpaceStation");
	model_reticle = Model::LoadFromObj("Box");
	model_Shooting = Model::LoadFromObj("Shooting");
	model_Force = Model::LoadFromObj("Force");

	Audio::GetInstance()->Initialize();

	Audio::GetInstance()->LoadWave("Bullet.wav");
	Audio::GetInstance()->LoadWave("BossDown.wav");
	Audio::GetInstance()->LoadWave("Damage.wav");
	Audio::GetInstance()->LoadWave("EnemyDown.wav");
	Audio::GetInstance()->LoadWave("GameOver.wav");
	Audio::GetInstance()->LoadWave("GameScene.wav");
	Audio::GetInstance()->LoadWave("GameClear.wav");
	

	//敵の初期化
	LoadEnemyPopData();
	EnemyInit();


	//3Dオブジェクト生成
	player = Player::Create(model_2, camera);
	Satellite_R = Player::Create(model_reticle, camera);
	Satellite_L = Player::Create(model_reticle, camera);

	PlayerPos = player->GetPosition();
	SatellitePos_R = Satellite_R->GetPosition();
	SatellitePos_L = Satellite_L->GetPosition();

	

	//天球生成
	CelestialSphere = Object3d::Create(model_sphere, camera);
	CelestialSphere->SetScale(ShereScale);

	Moon = Object3d::Create(model_Moon, camera);
	Moon->SetScale(MoonScale);
	Moon->SetPosition(MoonPosition);

	Station = Object3d::Create(model_Station, camera);
	Station->SetScale(StationScale);
	Station->SetPosition(StationPosition);
	StationRot.x = -StationRotation.x;

	//SpaceStation = Object3d::Create(model_SpaceStation, camera);
	//SpaceStation->SetRotation({ 90, 0, 0 });

	Shooting = Object3d::Create(model_Shooting, camera);
	Shooting->SetScale(DefaultShootingScale);
	Shooting->SetPosition(DefaultShootingPos);

	Force = Object3d::Create(model_Force, camera);
	Force->SetScale(DefaultForceScale);
	Force->SetPosition(DefaultForcePos);

	//レティクル生成
	//Reticle = Reticle::Create(model_reticle, camera);
	//Reticle->SetScale({ 0.5, 0.5, 0.5 });

	/*object1->PlayAnimation();*/

	//パーティクル生成
	particle = Particle::Create(L"Resources/Image/effect1.png", camera);
	particle->Update();

	EnemyPart = Particle::Create(L"Resources/Image/effect3.png", camera);
	EnemyPart->Update();

	//演出タイマー初期化
	WaitTimer = TimerReset;
	BossTimer = DefaultBossTimer;
	RangeTime = TimerReset;

	//ステージスタート時UIの初期化
	Stage_1->position_.x = DefaultStage1Pos.x;
	Stage_1->position_.y = DefaultStage1Pos.y;
	Go->position_.y = DefaultGoPos.y;

	BossUI_U->SetColor(BossUIColor);
	BossUI_U_2->SetColor(BossUIColor);
	BossUI_D->SetColor(BossUIColor);
	BossUI_D_2->SetColor(BossUIColor);

	BossUI_U->position_.x = BossUIPos_1.x;
	BossUI_U->position_.y = BossUIPos_1.y;
	BossUI_U_2->position_.x = -BossUIPos_2.x;
	BossUI_U_2->position_.y = BossUIPos_2.y;

	BossUI_D->position_.x = BossUIPos_1.x;
	BossUI_D->position_.y = -BossUIPos_1.y;
	BossUI_D_2->position_.x = BossUIPos_2.x;
	BossUI_D_2->position_.y = -BossUIPos_2.y;

	//最初の目的座標
	Stage_1->PointPos.x = Stage1PointPos.x;
	Stage_1->PointPos.y = Stage1PointPos.y;
	Go->PointPos.y = GOPointPos.y;

	BossUI_U->PointPos.x = BossUIPointPos_1.x;
	BossUI_U->PointPos.y = BossUIPointPos_1.y;
	BossUI_U_2->PointPos.x = BossUIPointPos_2.x;
	BossUI_U_2->PointPos.y = BossUIPointPos_2.y;

	BossUI_D->PointPos.x = -BossUIPointPos_1.x;
	BossUI_D->PointPos.y = BossUIPointPos_1.y;
	BossUI_D_2->PointPos.x = BossUIPointPos_2.x;
	BossUI_D_2->PointPos.y = BossUIPointPos_2.y;

	LoadBG->color_.w = DefaultAlpha_Max;

	//スケールのセット
	player->Object3d::SetScale(PlayerScale);
	Satellite_R->Object3d::SetScale(PlayerScale);
	Satellite_L->Object3d::SetScale(PlayerScale);

	//ボスのパーティクルの発生カウントをリセット
	//PartCount = 0;

	//コントローラー初期化
	InitInput();


	Audio::GetInstance()->PlayWave("GameScene.wav", 0.1f, true);
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
	//プレイヤーの座標を取得
	PlayerPos = player->GetPosition();

	PlayerRot = player->GetRotation();

	if (!LoadBG->color_.w <= 0.0f)
	{
		LoadBG->color_.w -= 0.02f;

		LoadBG->Update();
	}

	//テキストの座標を取得
	ShootingPos = Shooting->GetPosition();

	ForcePos = Force->GetPosition();

	Force->SetPosition(ForcePos);

	if (LoadBG->color_.w <= 0.0f)
	{
		LoadBG->color_.w = 0.0f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && GameStart == false)
	{
		GameStart = true;
	}

	if (GameStart == true)
	{
		MoveTitle();

		RangeTime++;

		if (RangeTime > 100)
		{
			Start();
		}
	}

		//UI->position_.x = Reticle->MousePosition.x;
		//UI->position_.y = Reticle->MousePosition.y;

		

	SatellitePos_R.x = PlayerPos.x + SatelliteRange.x;
	SatellitePos_R.y = PlayerPos.y - SatelliteRange.y;
	SatellitePos_R.z = PlayerPos.z;

	SatellitePos_L.x = PlayerPos.x - SatelliteRange.x;
	SatellitePos_L.y = PlayerPos.y - SatelliteRange.y;
	SatellitePos_L.z = PlayerPos.z;

	

		if (Input::GetInstance()->PushKey(DIK_P))
		{
			UI->BarSize.x += 1;
			UI->BarSize.y += 1;

			UI->SetSize(BarSize);
		}

		//攻撃時SE再生
		if (player->MoveCanFlag == true && Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			Audio::GetInstance()->PlayWave("Bullet.wav", Attack_Volume, false);
		}

		if (player->MoveCanFlag == true && IsButtonDown(ButtonKind::Button_B))
		{
			Audio::GetInstance()->PlayWave("Bullet.wav", Attack_Volume, false);
		}

		UI->GetSize();

		UI->Update();


		//シーン遷移
		//敵のHPが0になったらゲームクリア
		if (Boss)
		{
			if (Boss->GetDeathFlag() == true && BossTimer <= 0)
			{
				if (WaitTimer < 120)
				{
					WaitTimer += 1;
				}

				if (WaitTimer >= 120)
				{
					WaitTimer = 0;
					Audio::GetInstance()->StopWave("GameScene.wav");
					SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
				}
			}
		}
		//プレイヤーのHPが0になったらゲームオーバー
		if (player->HP <= 0)
		{
			particle->CreateParticleInfo(10, PlayerPos, 2.0f, 30, 2.0f, 0.0f);
			particle->DeathParticle(10, PlayerPos, 2.0f, 30, 5.0f, 0.0f);

			GameOver();

			if (WaitTimer < 100)
			{
				WaitTimer += 1;
			}

			if (WaitTimer >= 100)
			{
				WaitTimer = 0;
				Audio::GetInstance()->StopWave("GameScene.wav");
				SceneManager::GetInstance()->ChangeScene("GAMEOVER");
			}

		}


		//3Dオブジェクトの更新
		player->SetPosition(PlayerPos);
		player->SetRotation(PlayerRot);
		player->Update();

		Satellite_R->SetPosition(SatellitePos_R);
		Satellite_R->Update();

		Satellite_L->SetPosition(SatellitePos_L);
		Satellite_L->Update();

		/*Reticle->Update();*/

		if (player->MoveCanFlag == true)
		{
			MoonRot = Moon->GetRotation();
			MoonPos = Moon->GetPosition();

			MoonPos.z -= 0.05f;

			Moon->SetPosition(MoonPos);

			StationRot = Station->GetRotation();
			StationPos = Station->GetPosition();

			StationPos.z -= 0.1f;

			Station->SetPosition(StationPos);

			Attack(PlayerPos);

			if (player->Level >= 2)
			{
				Attack(SatellitePos_R);
			}

			if (player->Level >= 3)
			{
				Attack(SatellitePos_L);
			}
		}


		MeteorTimer += 1;

		if (MeteorTimer >= 20)
		{
			MeteorMove();
			MeteorTimer = 0;
		}

		if (GameStart == true)
		{
			UpdateEnemyPopCommands();
		}

		//敵の更新
		for (std::unique_ptr<Enemy>& enemy : enemys)
		{
			if (enemy->GetDeathFlag() == false)
			{
				EnemyPos = enemy->GetPosition();
				enemy->Update();

				EnemyFire = enemy->GetFireTime();

				EnemyFire--;

				enemy->SetFireTime(EnemyFire);

				if (enemy->GetFireTime() <= 0)
				{
					EnemyAttack(EnemyPos);

					//発射タイマーを初期化
					enemy->SetFireTime(enemy->GetIntervalTime());
				}
			}

		}

		//ボスの更新
		if (Boss)
		{
			Boss->Update();

			if (Boss->GetDeathFlag() == false)
			{
				if (Boss->GetDeathEffect() == false)
				{
					BossFire--;
				}

				if (BossFire <= 0)
				{
					EnemyAttack(Boss->GetPosition());

					//発射タイマーを初期化
					BossFire = Boss->GetIntervalTime();
				}

			}
		}


		//弾更新
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			bullet->Update(PlayerPos);
		}

		for (std::unique_ptr<Bullet>& CoreR_bullet : CoreR_bullets)
		{
			CoreR_bullet->Update(SatellitePos_R);
		}

		for (std::unique_ptr<Bullet>& CoreL_bullet : CoreL_bullets)
		{
			CoreL_bullet->Update(SatellitePos_L);
		}

		//敵の弾更新
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
		{
			//仮として引数をプレイヤーの座標にしている
			bullet->Update(PlayerPos, PlayerPos);
		}

		//ボスの弾更新
		for (std::unique_ptr<BossBullet>& bullet : bossbullets)
		{
			//仮として引数をプレイヤーの座標にしている
			bullet->Update(PlayerPos, PlayerPos);
		}

		//デスフラグの立った弾の削除
		//プレイヤー弾
		bullets.remove_if([](std::unique_ptr<Bullet>& bullet)
			{
				return bullet->GetDeathFlag();
			});

		//コア弾
		CoreR_bullets.remove_if([](std::unique_ptr<Bullet>& CoreR_bullet)
			{
				return CoreR_bullet->GetDeathFlag();
			});

		CoreL_bullets.remove_if([](std::unique_ptr<Bullet>& CoreL_bullet)
			{
				return CoreL_bullet->GetDeathFlag();
			});

		//敵弾
		enemybullets.remove_if([](std::unique_ptr<EnemyBullet>& Enemybullet)
			{
				return Enemybullet->GetDeathFlag();
			});

		//ボス弾
		bossbullets.remove_if([](std::unique_ptr<BossBullet>& Bossbullet)
			{
				return Bossbullet->DeathGetter();
			});

		//オブジェクト
		for (std::unique_ptr<DefenceObject>& object : objects)
		{
			float speed = 50.0f;

			//発射タイマーを初期化
			for (std::unique_ptr<DefenceObject>& object : objects)
			{
				if (MeteorPos.z < -10)
				{
					object->SetDeathFlag(true);
				}
			}

			object->Update(speed);
		}


		//当たり判定
		//プレイヤーの弾と敵の当たり判定
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				EnemyPos = enemy->GetPosition();
				//当たり判定確認
				if (enemy->GetDeathFlag() == false)
				{
					if (CheckCollision(bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//パーティクルを生成
						EnemyPart->CreateParticleInfo(50, EnemyPos, 2.0f, 30, 2.0f, 0.0f);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						bullet->SetDeathFlag(true);
						enemy->SetDeathFlag(true);
						player->EXP += 1;
					}
				}
			}
		}

		//コアと敵の当たり判定
		//右
		for (std::unique_ptr<Bullet>& CoreR_bullet : CoreR_bullets)
		{
			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				EnemyPos = enemy->GetPosition();
				//当たり判定確認
				if (enemy->GetDeathFlag() == false)
				{
					if (CheckCollision(CoreR_bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//パーティクルを生成
						EnemyPart->CreateParticleInfo(50, EnemyPos, 2.0f, 30, 2.0f, 0.0f);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						CoreR_bullet->SetDeathFlag(true);
						enemy->SetDeathFlag(true);
						player->EXP += 1;
					}
				}
			}
		}

		//左
		for (std::unique_ptr<Bullet>& CoreL_bullet : CoreL_bullets)
		{
			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				EnemyPos = enemy->GetPosition();
				//当たり判定確認
				if (enemy->GetDeathFlag() == false)
				{
					if (CheckCollision(CoreL_bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//パーティクルを生成
						EnemyPart->CreateParticleInfo(50, EnemyPos, 2.0f, 30, 2.0f, 0.0f);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						CoreL_bullet->SetDeathFlag(true);
						enemy->SetDeathFlag(true);
						player->EXP += 1;
					}
				}
			}
		}

		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
		{
			//自機と敵の弾当たり判定確認
			if (CheckCollision(PlayerPos, bullet->GetPosition(), 2.0f, 2.0f) == true)
			{
				HitFlag = true;
				DamageEffectflag = true;

				//パーティクルを生成
				particle->CreateParticleInfo(10, PlayerPos, 2.0f, 30, 2.0f, 0.0f);
				//ダメージSEを再生
				Audio::GetInstance()->PlayWave("Damage.wav", 0.1f, false);
				//プレイヤーのHPをデクリメントして敵の弾のデスフラグを上げる
				player->HP -= 1;
				bullet->SetDeathFlag(true);
			}
		}


		//プレイヤーの弾とボスの当たり判定
		if (Boss)
		{
			if (Boss->GetDeathFlag() == false)
			{
				for (std::unique_ptr<Bullet>& bullet : bullets)
				{
					if (CheckCollision(bullet->GetPosition(), Boss->GetPosition(), 2.0f, 6.0f) == true)
					{
						//パーティクルを生成
						particle->CreateParticleInfo(10, Boss->GetPosition(), 2.0f, 30, 1.0f, 0.0f);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						Boss->SetHP(Boss->GetHP()-1);
						if (Boss->GetHP() <= 0)
						{
							Audio::GetInstance()->PlayWave("BossDown.wav", 0.1f, false);
						}
						bullet->SetDeathFlag(true);
					}
				}
			}

			if (Boss->GetHP() <= 0)
			{
				BossPartTimer += 1;

				if (BossPartTimer >= 20 && Boss->GetDeathFlag() == false)
				{
					BossPartTimer = 0;
					PartCount += 1;
					particle->CreateParticleInfo(10, Boss->GetPosition(), 2.0f, 50, 8.0f, 0.0f);
				}

				if (PartCount >= 5)
				{
					particle->CreateParticleInfo(60, Boss->GetPosition(), 4.0f, 50, 15.0f, 0.0f);
					PartCount = 0;
					Boss->SetDeathFlag(true);
				}

			}
		}

		//ボスの弾とプレイヤーの当たり判定
		if (Boss)
		{
			for (std::unique_ptr<BossBullet>& bullet : bossbullets)
			{
				//自機と敵の弾当たり判定確認
				if (CheckCollision(PlayerPos, bullet->GetPosition(), 2.0f, 2.0f) == true)
				{
					if (player->HP == 1)
					{
						//ボス撃破時はパーティクルを派手に演出
						particle->CreateParticleInfo(50, PlayerPos, 2.0f, 50, 5.0f, 0.0f);
						Audio::GetInstance()->PlayWave("BossDown.wav", 0.1f, false);
					}
					player->HP -= 1;
					bullet->DeathFlag = true;
				}
			}
		}

		if (HitFlag == true)
		{ 
			ShakeTimer++;

			if (ShakeTimer < 5)
			{
				camera->CameraMoveEyeVector(Shake);
				Shake.x *= -1.0f;
			}

			if (ShakeTimer >= 5)
			{
				HitFlag = false;
				ShakeTimer = 0;
				camera->SetEye(CameraDefault);
			}
		}

		if (DamageEffectflag == true)
		{
			Damage();
		}
		//天球更新
		CelestialSphere->Update();

		MoonRot.y += 0.2f;

		Moon->SetRotation(MoonRot);


		Moon->Update();

		//StationRot.y += 0.03f;

		Station->SetRotation(StationRot);

		Station->Update();
		//SpaceStation->Update();
		Shooting->Update();
		Force->Update();

		

		//レティクル更新
		//Reticle->Update(P->position_);

		//FBXオブジェクトの更新
		//object1->Update();


		//スプライトの更新

		UpdateSprite();

		if (Input::GetInstance()->PushKey(DIK_J) || Input::GetInstance()->PushKey(DIK_L) || Input::GetInstance()->PushKey(DIK_I) || Input::GetInstance()->PushKey(DIK_K))
		{
			if (Input::GetInstance()->PushKey(DIK_J))
			{
				camera->CameraMoveEyeVector({ -1.0f, 0, 0 });
			}

			if (Input::GetInstance()->PushKey(DIK_L))
			{
				camera->CameraMoveEyeVector({ 1.0f, 0, 0 });
			}

			if (Input::GetInstance()->PushKey(DIK_I))
			{
				camera->CameraMoveEyeVector({ 0, 1.0f, 0 });
			}

			if (Input::GetInstance()->PushKey(DIK_K))
			{
				camera->CameraMoveEyeVector({ 0, -1.0f, 0 });
			}
		}

		//カメラの更新
		camera->Update();

		//パーティクル更新
		particle->Update();
		
		EnemyPart->Update();

		UpdateInput();

	//}

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
	
	

	if (Boss)
	{

		if (Boss->GetDeathFlag() == false)
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
		if (enemy->GetDeathFlag() == false)
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

	for (std::unique_ptr<DefenceObject>& object : objects)
	{
		object->Draw();
	}

	//天球の描画
	CelestialSphere->Draw();

	Moon->Draw();
	Station->Draw();
	//SpaceStation->Draw();
	Shooting->Draw();
	Force->Draw();

	//レティクル描画
	//Reticle->Draw();

	//FBXオブジェクトの描画
	//object1->Draw(cmdList);

	if (player->Level >= 2)
	{
		Satellite_R->Draw();
	}

	if (player->Level >= 3)
	{
		Satellite_L->Draw();
	}

	player->Draw();

	//3Dオブジェクトの描画後処理
	Object3d::PostDraw();

	Particle::PreDraw();

	
	particle->Draw();

	EnemyPart->Draw();
	

	Particle::PostDraw();


	////スプライトの共通コマンド
	SpriteCommon::GetInstance()->PreDraw();
	
	//UI->Draw();

	

	Stage_1->Draw();
	Go->Draw();
	LoadBG->Draw();
	


		if (Go->ComFlag_2 == true)
		{
			if (player->HP == 5)
			{
				HP_5->Draw();
			}

			if (player->HP == 4)
			{
				HP_4->Draw();
			}

			if (player->HP == 3)
			{
				HP_3->Draw();
			}

			if (player->HP == 2)
			{
				HP_2->Draw();
			}

			if (player->HP == 1)
			{
				HP_1->Draw();
			}

			if (player->HP == 0)
			{
				HP_0->Draw();
			}
		}

	DrawSprite();

}

void GameScene::EnemyInit()
{
	//発射タイマーを初期化
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->SetFireTime(enemy->GetIntervalTime());
	}
}

void GameScene::BossInit()
{
	Boss->SetFireTime(Boss->GetIntervalTime());
}

void GameScene::EnemyUpdate(XMFLOAT3 enemyPos)
{
	EnemyBulletTimer--;
	
}

void GameScene::Attack(XMFLOAT3 StartPos)
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || IsButtonPush(ButtonKind::Button_B))
	{
		//弾を生成し初期化
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet = Bullet::Create(model_Bullet, camera, StartPos, ReticlePos/*Reticle->position_*/);

		particle->FireParticle(30, StartPos, 2.0f, 10, 1.0f, 0.0f);

		//弾を登録
		bullets.push_back(std::move(newBullet));
	}


	//if (IsButtonPush(ButtonKind::Button_B))
	//{
	//	//弾を生成し初期化
	//	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	//	newBullet = Bullet::Create(model_Bullet, camera, StartPos, ReticlePos/*Reticle->position_*/);

	//	//弾を登録
	//	bullets.push_back(std::move(newBullet));
	//}
}

void GameScene::EnemyAttack(XMFLOAT3 EnemyPos)
{
	
		//弾を生成し初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet = EnemyBullet::Create(model_EnemyBullet, camera, EnemyPos,PlayerPos);

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
			Boss = Boss::Create(model_Boss, camera, EnemyPos);
		}

		//POPコマンド
		else if (word.find("OBJECT_POP") == 0)
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
			std::unique_ptr<DefenceObject> newObject = std::make_unique<DefenceObject>();
			newObject = DefenceObject::Create(model_Meteor, camera, EnemyPos);

			objects.push_back(std::move(newObject));
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

		else if (word.find("EVENT_S") == 0)
		{
			getline(line_stream, word, ',');

			BossFlag_S = true;
		}

		else if (word.find("EVENT_E") == 0)
		{
			getline(line_stream, word, ',');

			BossFlag_E = true;
			BossFlag_S = false;
		}

		else if (word.find("UI_FLAGDOWN") == 0)
		{
			getline(line_stream, word, ',');

			BossUIDrawFlag = false;
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
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/BackGround.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(2, L"Resources/Image/Reticle.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(2, L"Resources/Image/Reticle.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(3, L"Resources/Image/HP_5.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(4, L"Resources/Image/HP_4.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(5, L"Resources/Image/HP_3.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(6, L"Resources/Image/HP_2.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(7, L"Resources/Image/HP_1.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(8, L"Resources/Image/HP_0.png");
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
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(25, L"Resources/Image/ExpBar/ExpUI_0.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(26, L"Resources/Image/ExpBar/ExpUI_1.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(27, L"Resources/Image/ExpBar/ExpUI_2.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(28, L"Resources/Image/ExpBar/ExpUI_3.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(29, L"Resources/Image/ExpBar/ExpUI_4.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(30, L"Resources/Image/ExpBar/ExpUI_5.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(31, L"Resources/Image/ExpBar/Level_1.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(32, L"Resources/Image/ExpBar/Level_2.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(33, L"Resources/Image/ExpBar/Level_3.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(34, L"Resources/Image/Stage_1.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(35, L"Resources/Image/Go.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(36, L"Resources/Image/Rule.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(37, L"Resources/Image/Warning_D.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(38, L"Resources/Image/Warning_U.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(39, L"Resources/Image/Warning.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(40, L"Resources/Image/LoadBG.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(41, L"Resources/Image/DamageEffect.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(42, L"Resources/Image/GameStart.png");

	sprite = Sprite::Create(1, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	UI = Sprite::Create(2, { 1, 1, 1, 1 }, { 0.5, 0.5 }, false, false);
	HP_0 = Sprite::Create(8, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	HP_1 = Sprite::Create(7, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	HP_2 = Sprite::Create(6, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	HP_3 = Sprite::Create(5, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	HP_4 = Sprite::Create(4, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	HP_5 = Sprite::Create(3, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_0 = Sprite::Create(9, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_1 = Sprite::Create(10, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_2 = Sprite::Create(11, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_3 = Sprite::Create(12, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_4 = Sprite::Create(13, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_5 = Sprite::Create(14, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_6 = Sprite::Create(15, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_7 = Sprite::Create(16, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_8 = Sprite::Create(17, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_9 = Sprite::Create(18, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_10 = Sprite::Create(19, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_11 = Sprite::Create(20, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_12 = Sprite::Create(21, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_13 = Sprite::Create(22, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_14 = Sprite::Create(23, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	BossHP_15 = Sprite::Create(24, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	ExpBar_0 = Sprite::Create(25, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	ExpBar_1 = Sprite::Create(26, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	ExpBar_2 = Sprite::Create(27, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	ExpBar_3 = Sprite::Create(28, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	ExpBar_4 = Sprite::Create(29, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	ExpBar_5 = Sprite::Create(30, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	Level_1 = Sprite::Create(31, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	Level_2 = Sprite::Create(32, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	Level_3 = Sprite::Create(33, { 1, 1, 1, 1 }, { 0, 0 }, false, false);

	Stage_1 = Sprite::Create(34, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	Go = Sprite::Create(35, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	Rule = Sprite::Create(36, { 1, 1, 1, 1 }, { 0, 0 }, false, false);

	BossUI_U = Sprite::Create(37, { 1, 1, 1, 0.2f }, { 0, 0 }, false, false);
	BossUI_D = Sprite::Create(38, { 1, 1, 1, 0.5f }, { 0, 0 }, false, false);
	BossUI_U_2 = Sprite::Create(37, { 1, 1, 1, 0.5f }, {0, 0}, false, false);
	BossUI_D_2 = Sprite::Create(38, { 1, 1, 1, 0.5f }, { 0, 0 }, false, false);
	Warning = Sprite::Create(39, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	LoadBG = Sprite::Create(40, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	DamageEffect = Sprite::Create(41, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);

	//タイトルUI
	StartUI = Sprite::Create(42, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	StartUI->SetSize({ 100,100 });
	StartUI->SetPosition({ 500, 500, 0 });
}

void GameScene::UpdateSprite()
{
	sprite->Update();
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

	Stage_1->Update();
	Go->Update();
	BossUI_U->Update();
	BossUI_D->Update();
	BossUI_U_2->Update();
	BossUI_D_2->Update();
	Warning->Update();
	DamageEffect->Update();

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

	if (BossFlag_S == true)
	{
		if (WarningFlag == false)
		{
			Warning->color_.w -= 0.01f;
		}

		if (Warning->color_.w <= 0.0)
		{
			WarningFlag = true;
		}

		if (WarningFlag == true)
		{
			Warning->color_.w += 0.01f;
		}

		if (Warning->color_.w >= 1.0)
		{
			WarningFlag = false;
		}
	}

	if (GameStart == false)
	{
		StartUI->Update();
	}
}

void GameScene::DrawSprite()
{
	if (DamageEffectflag == true)
	{
		DamageEffect->Draw();
	}

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
		if (Boss->GetHP() == 15)
		{
			BossHP_15->Draw();
		}

		if (Boss->GetHP() == 14)
		{
			BossHP_14->Draw();
		}

		if (Boss->GetHP() == 13)
		{
			BossHP_13->Draw();
		}

		if (Boss->GetHP() == 12)
		{
			BossHP_12->Draw();
		}

		if (Boss->GetHP() == 11)
		{
			BossHP_11->Draw();
		}

		if (Boss->GetHP() == 10)
		{
			BossHP_10->Draw();
		}

		if (Boss->GetHP() == 9)
		{
			BossHP_9->Draw();
		}

		if (Boss->GetHP() == 8)
		{
			BossHP_8->Draw();
		}

		if (Boss->GetHP() == 7)
		{
			BossHP_7->Draw();
		}

		if (Boss->GetHP() == 6)
		{
			BossHP_6->Draw();
		}

		if (Boss->GetHP() == 5)
		{
			BossHP_5->Draw();
		}

		if (Boss->GetHP() == 4)
		{
			BossHP_4->Draw();
		}

		if (Boss->GetHP() == 3)
		{
			BossHP_3->Draw();
		}

		if (Boss->GetHP() == 2)
		{
			BossHP_2->Draw();
		}

		if (Boss->GetHP() == 1)
		{
			BossHP_1->Draw();
		}

		if (Boss->GetHP() <= 0)
		{
			BossHP_0->Draw();
		}
	}
	
	//経験値UIの描画

	if (Go->ComFlag_2 == true)
	{
		if (player->EXP == 0)
		{
			ExpBar_0->Draw();
		}

		else if (player->EXP == 1)
		{
			ExpBar_1->Draw();
		}

		else if (player->EXP == 2)
		{
			ExpBar_2->Draw();
		}

		else if (player->EXP == 3)
		{
			ExpBar_3->Draw();
		}

		else if (player->EXP == 4)
		{
			ExpBar_4->Draw();
		}

		else if (player->EXP >= 5)
		{
			ExpBar_5->Draw();
		}

		//レベル
		if (player->Level == 1)
		{
			Level_1->Draw();
		}

		else if (player->Level == 2)
		{
			Level_2->Draw();
		}

		else if (player->Level >= 3)
		{
			Level_3->Draw();
		}

		//ルールUI
		Rule->Draw();
	}

	//ボス演出UI
	if (BossUIDrawFlag == true)
	{
		BossUI_U->Draw();
		BossUI_D->Draw();
		BossUI_U_2->Draw();
		BossUI_D_2->Draw();
		Warning->Draw();
	}

	if (BossFlag_S == true)
	{
		BossUIDrawFlag = true;

		BossUI_U->Vec.x = (BossUI_U->PointPos.x - BossUI_U->position_.x) / 100;
		BossUI_U->position_.x += BossUI_U->Vec.x;
		BossUI_U->Vec.y = (BossUI_U->PointPos.y - BossUI_U->position_.y) / 30;
		BossUI_U->position_.y += BossUI_U->Vec.y;

		BossUI_U_2->Vec.x = (BossUI_U_2->PointPos.x - BossUI_U_2->position_.x) / 100;
		BossUI_U_2->position_.x += BossUI_U_2->Vec.x;
		BossUI_U_2->Vec.y = (BossUI_U_2->PointPos.y - BossUI_U_2->position_.y) / 30;
		BossUI_U_2->position_.y += BossUI_U_2->Vec.y;

		BossUI_D->Vec.x = (BossUI_D->PointPos.x - BossUI_D->position_.x) / 100;
		BossUI_D->position_.x += BossUI_D->Vec.x;
		BossUI_D->Vec.y = (BossUI_D->PointPos.y - BossUI_D->position_.y) / 30;
		BossUI_D->position_.y += BossUI_D->Vec.y;

		BossUI_D_2->Vec.x = (BossUI_D_2->PointPos.x - BossUI_D_2->position_.x) / 100;
		BossUI_D_2->position_.x += BossUI_D_2->Vec.x;
		BossUI_D_2->Vec.y = (BossUI_D_2->PointPos.y - BossUI_D_2->position_.y) / 30;
		BossUI_D_2->position_.y += BossUI_D_2->Vec.y;

	}

	if (BossFlag_E == true)
	{
		BossUI_U->PointPos.y = 100;
		BossUI_U_2->PointPos.y = 100;
		BossUI_D->PointPos.y = -100;
		BossUI_D_2->PointPos.y = -100;

		EndUIFlag = true;
	}

	if (EndUIFlag == true)
	{

		BossUI_U->Vec.y = (BossUI_U->PointPos.y - BossUI_U->position_.y) / 30;
		BossUI_U->position_.y += BossUI_U->Vec.y;

		BossUI_U_2->Vec.y = (BossUI_U_2->PointPos.y - BossUI_U_2->position_.y) / 30;
		BossUI_U_2->position_.y += BossUI_U_2->Vec.y;

		BossUI_D->Vec.y = (BossUI_D->PointPos.y - BossUI_D->position_.y) / 30;
		BossUI_D->position_.y += BossUI_D->Vec.y;

		BossUI_D_2->Vec.y = (BossUI_D_2->PointPos.y - BossUI_D_2->position_.y) / 30;
		BossUI_D_2->position_.y += BossUI_D_2->Vec.y;
	}

	//タイトルUI
	if (GameStart == false)
	{
		StartUI->Draw();
	}
}

void GameScene::Start()
{
	//StartTimer += 1;
	player->MoveCanFlag = false;
	Stage_1->Vec.x = (Stage_1->PointPos.x - Stage_1->position_.x) / 20;
	Stage_1->position_.x += Stage_1->Vec.x;

	if (Stage_1->position_.x <= Stage_1->PointPos.x + 10)
	{
		Stage_1->PointPos.x = -400;
		Stage_1->PointPos.y = 285;
	}

	if (Stage_1->position_.x <= Stage_1->PointPos.x + 10)
	{
		Go->Vec.y = (Go->PointPos.y - Go->position_.y) / 20;
		Go->position_.y += Go->Vec.y;
		Go->ComFlag = true;
	}

	if (Go->position_.y >= Go->PointPos.y - 10 && Go->ComFlag == true)
	{
		Go->PointPos.y = 720;

		Go->Vec.y = (Go->PointPos.y - Go->position_.y) / 30;
		Go->position_.y += Go->Vec.y;
		Go->ComFlag_2 = true;
	}

	if (Go->position_.y >= Go->PointPos.y - 20 && Go->ComFlag_2 == true)
	{
		if (PlayerPos.z < 0)
		{
			//P->position_.z += 3;
			PlayerPos.z += 3;
			player->SetPosition(PlayerPos);

		}
		player->MoveCanFlag = true;

	}

}

void GameScene::Damage()
{
	DamageEffectflag = true;

	DamageEffect->color_.w -= 0.05f;
	
	if (DamageEffect->color_.w <= 0)
	{
		DamageEffect->color_.w = 1;
		DamageEffectflag = false;
	}	
}

void GameScene::GameOver()
{
	VY += gravity;

	PlayerPos.x += 0.5f;
	PlayerPos.y += VY * 0.2f;

	PlayerRot.x += 0.5f;
}

void GameScene::MeteorMove()
{
	float MeteorX = rand()%(100 - (-100) + 1) + (-100);
	float MeteorY = rand() % (50 - (-50) + 1) + (-50);
	float MeteorZ = 200;

	MeteorPos = { MeteorX, MeteorY, MeteorZ };

	std::unique_ptr<DefenceObject> newObject = std::make_unique<DefenceObject>();
	newObject = DefenceObject::Create(model_Meteor, camera, MeteorPos);

	objects.push_back(std::move(newObject));
}

void GameScene::BossDeath()
{
	BossPartTimer += 1;

	if (BossPartTimer >= 20 && Boss->GetDeathFlag() == false)
	{
		BossPartTimer = 0;
		PartCount += 1;
		particle->CreateParticleInfo(10, Boss->GetPosition(), 2.0f, 50, 8.0f, 0.0f);
	}

	if (PartCount >= 5)
	{
		particle->CreateParticleInfo(60, Boss->GetPosition(), 4.0f, 50, 15.0f, 0.0f);
		PartCount = 0;
		Boss->SetDeathFlag(true);
	}
}

void GameScene::MoveTitle()
{
	float ShootingTarget = -120.0f;
	float ForceTarget = 70.0f;
	float Easing = 0.03f;

	ShootingPos.x += (ShootingTarget - ShootingPos.x) * Easing;
	ForcePos.x += (ForceTarget - ForcePos.x) * Easing;

	Shooting->SetPosition(ShootingPos);
	Force->SetPosition(ForcePos);
}



