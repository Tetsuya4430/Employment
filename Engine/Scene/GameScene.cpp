#include "GameScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
#include "FrameWork.h"
#include "Controller.h"

#include "imgui.h"

#include <DirectXMath.h>
#include <iomanip>

using namespace std;
using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	
	
	LoadSprite();

	//OBJ���烂�f���f�[�^��ǂݍ���
	model_2 = Model::LoadFromObj("Player");
	model_Bullet = Model::LoadFromObj("Bullet");
	model_EnemyBullet = Model::LoadFromObj("EnemyBullet");
	model_Enemy = Model::LoadFromObj("Enemy_1");
	model_DefenceEnemy = Model::LoadFromObj("Enemy_2");
	model_Boss = Model::LoadFromObj("Boss");
	model_BossCore = Model::LoadFromObj("BossCore");
	model_sphere = Model::LoadFromObj("CelestialSphere");
	model_Moon = Model::LoadFromObj("Moon");
	model_Mars = Model::LoadFromObj("Mars");
	model_Neptune = Model::LoadFromObj("Neptune");
	model_Meteor = Model::LoadFromObj("Meteor");
	model_Station = Model::LoadFromObj("Station");
	model_SpaceStation = Model::LoadFromObj("SpaceStation");
	model_reticle = Model::LoadFromObj("Box");
	model_Shooting = Model::LoadFromObj("Shooting");
	model_Force = Model::LoadFromObj("Force");

	Audio::GetInstance()->Initialize();

	Audio::GetInstance()->LoadWave("Bullet.wav");
	Audio::GetInstance()->LoadWave("BossDown.wav");
	Audio::GetInstance()->LoadWave("Damage.wav");
	Audio::GetInstance()->LoadWave("Reflect.wav");
	Audio::GetInstance()->LoadWave("EnemyDown.wav");
	Audio::GetInstance()->LoadWave("GameOver.wav");
	Audio::GetInstance()->LoadWave("GameScene.wav");
	Audio::GetInstance()->LoadWave("GameClear.wav");
	

	//�G�̏�����
	LoadEnemyPopData();
	EnemyInit();


	//3D�I�u�W�F�N�g����
	player = Player::Create(model_2, camera);
	Satellite_R = Player::Create(model_reticle, camera);
	Satellite_L = Player::Create(model_reticle, camera);

	PlayerPos = player->GetPosition();
	SatellitePos_R = Satellite_R->GetPosition();
	SatellitePos_L = Satellite_L->GetPosition();

	//�Q�[���X�^�[�g����t���O�̃��Z�b�g
	player->SetGameStart(false);
	PlayerMoveFlag = false;

	//�V������
	CelestialSphere = Object3d::Create(model_sphere, camera);
	CelestialSphere->SetScale(ShereScale);

	Moon = Object3d::Create(model_Moon, camera);
	Moon->SetScale(MoonScale);
	Moon->SetPosition(MoonPosition);

	Mars = Object3d::Create(model_Mars, camera);
	Mars->SetScale(MarsScale);
	Mars->SetPosition(MarsPosition);

	Neptune = Object3d::Create(model_Neptune, camera);
	Neptune->SetScale(NeptuneScale);
	Neptune->SetPosition(NeptunePosition);

	Station = Object3d::Create(model_Station, camera);
	Station->SetScale(StationScale);
	Station->SetPosition(StationPosition);
	StationRot.x = -StationRotation.x;


	Shooting = Object3d::Create(model_Shooting, camera);
	Shooting->SetScale(DefaultShootingScale);
	Shooting->SetPosition(DefaultShootingPos);

	Force = Object3d::Create(model_Force, camera);
	Force->SetScale(DefaultForceScale);
	Force->SetPosition(DefaultForcePos);

	CameraObject = Object3d::Create(model_reticle, camera);
	CameraObject->SetScale(DefaultCameraObjScale);

	CameraPos = camera->GetEye();

	CameraObject->SetPosition(CameraPos);


	//�p�[�e�B�N������
	Particle::GetInstance()->LoadTexture(1, L"Resources/Image/effect1.png");
	particle = Particle::Create(1, camera);

	Particle::GetInstance()->LoadTexture(2, L"Resources/Image/Levelup.png");
	particle_Red = Particle::Create(2, camera);


	//���o�^�C�}�[������
	WaitTimer = TimerReset;
	BossTimer = DefaultBossTimer;
	RangeTime = TimerReset;

	//�X�e�[�W�X�^�[�g��UI�̏�����
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

	//�ŏ��̖ړI���W
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


	//�X�P�[���̃Z�b�g
	player->Object3d::SetScale(PlayerScale);
	Satellite_R->Object3d::SetScale(PlayerScale);
	Satellite_L->Object3d::SetScale(PlayerScale);

	//�t�F�[�h�C���p�̉摜�̃A���t�@�l���f�t�H���g�ɐݒ�
	LoadBG->color_.w = DefaultAlpha_Max;

	//�^�C�g���I�u�W�F�N�g���f�t�H���g�ʒu�ɐݒ�
	ShootingPos = DefaultShootingPos;
	ForcePos = DefaultForcePos;


	//�J�����̈ʒu�����Z�b�g
	camera->SetEye({ 0, 0, -100.0f });

	//�R���g���[���[������
	InitInput();

	//�e�q�֌W������
	//player->SetParent(CameraObject);

	//�Q�[��BGM�̍Đ�
	Audio::GetInstance()->PlayWave("GameScene.wav", GameBGM_Volume, true);
}

void GameScene::Finalize()
{
	//���f���̉��
	delete model_1;
	delete model_2;
}

void GameScene::Update()
{
	CameraPos = camera->GetEye();
	//�v���C���[�̍��W���擾
	PlayerPos = player->GetPosition();

	PlayerRot = player->GetRotation();

	particle->JettParticle(2, { PlayerPos.x, PlayerPos.y , PlayerPos.z - 12 }, PartVel, 20, 1.0f, Value_Zero, PartSpeed, { 1.0, 0.654, 0.1, 1.0 });

	
	if (!LoadBG->color_.w <= Value_Zero)
	{
		LoadBG->color_.w -= BG_Decriment;

		LoadBG->Update();
	}

	if (LoadBG->color_.w <= Value_Zero)
	{
		LoadBG->color_.w = Value_Zero;
	}


	//�e�L�X�g�̍��W���擾
	ShootingPos = Shooting->GetPosition();

	ForcePos = Force->GetPosition();

	Force->SetPosition(ForcePos);


	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && GameStart == false)
	{
		GameStart = true;
	}



	if (GameStart == true)
	{
		MoveTitle();

		//�����W�^�C���̃C���N�������g
		RangeTime++;

		//�ҋ@�J�E���g����������Q�[���X�^�[�g
		if (RangeTime > RangeCount)
		{
			Start();
		}
	}


		

	SatellitePos_R.x = PlayerPos.x + SatelliteRange.x;
	SatellitePos_R.y = PlayerPos.y - SatelliteRange.y;
	SatellitePos_R.z = PlayerPos.z;

	SatellitePos_L.x = PlayerPos.x - SatelliteRange.x;
	SatellitePos_L.y = PlayerPos.y - SatelliteRange.y;
	SatellitePos_L.z = PlayerPos.z;


		//�U����SE�Đ�
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


		//�V�[���J��
		//�G��HP��0�ɂȂ�����Q�[���N���A
		if (Boss)
		{
			if (Boss->GetDeathFlag() == true && BossTimer <= 0)
			{
				if (WaitTimer < WaitCount)
				{
					WaitTimer++;
				}

				if (WaitTimer >= WaitCount)
				{
					WaitTimer = Value_Zero;
					Audio::GetInstance()->StopWave("GameScene.wav");
					SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
				}
			}
		}
		//�v���C���[��HP��0�ɂȂ�����Q�[���I�[�o�[
		if (player->HP <= Value_Zero)
		{
			particle->CreateParticleInfo(10, PlayerPos, 2.0f, 30, 2.0f, Value_Zero, { 1.000,0.470, 0.227, 1.0});
			particle->DeathParticle(10, PlayerPos, 2.0f, 30, 5.0f, Value_Zero, { 1.000,0.470, 0.227, 1.0 });

			GameOver();

			if (WaitTimer < WaitCount_GameOver)
			{
				WaitTimer++;
			}

			if (WaitTimer >= WaitCount_GameOver)
			{
				WaitTimer = Value_Zero;
				Audio::GetInstance()->StopWave("GameScene.wav");
				SceneManager::GetInstance()->ChangeScene("GAMEOVER");
			}

		}


		//3D�I�u�W�F�N�g�̍X�V
		player->SetPosition(PlayerPos);
		player->SetRotation(PlayerRot);
		player->Update();

		Satellite_R->SetPosition(SatellitePos_R);
		Satellite_R->Update();

		Satellite_L->SetPosition(SatellitePos_L);
		Satellite_L->Update();


		if (player->MoveCanFlag == true)
		{
			HpBarMove();

			CameraMove();

			MoonRot = Moon->GetRotation();
			MoonPos = Moon->GetPosition();

			MoonPos.z  += MoonVec.z;


			MarsRot = Mars->GetRotation();
			MarsPos = Mars->GetPosition();

			MarsPosition.z += MarsVec.z;

			Mars->SetPosition(MarsPosition);



			NeptuneRot = Neptune->GetRotation();
			NeptunePos = Neptune->GetPosition();

			NeptunePosition.z += NeptuneVec.z;

			Neptune->SetPosition(NeptunePosition);


			StationRot = Station->GetRotation();
			StationPos = Station->GetPosition();

			StationPos.z += StationVec.z;

			Station->SetPosition(StationPos);

			Attack(PlayerPos);

			if (player->LevelFlag == true)
			{
				LevelCount++;

				if (LevelCount == 30)
				{
					LevelCount = Value_Zero;
					player->SetLelelflag(false);
				}

				particle_Red->LevelUpParticle(3, particle->GetLevelUpPartPos(), player->GetPosition(), 0.2f, 80, 3.0f, Value_Zero, { 0.1f, 1.0f, 0.1f, 1.0f });
				particle_Red->LevelUpParticle(3, particle->GetLevelUpPartPos_2(), player->GetPosition(), 0.2f, 80, 3.0f, Value_Zero, { 0.1f, 1.0f, 0.1f, 1.0f });
				//particle->PlayerLevelUpParticle(5, PlayerPos, 2.0f, 30, 5.0f, 0.0f, {0.078, 1.0, 0.654, 1.0});
			}

			if (player->Level >= 2)
			{
				Attack(SatellitePos_R);
			}

			if (player->Level >= 3)
			{
				Attack(SatellitePos_L);
			}
		}


		MeteorTimer++;

		if (MeteorTimer >= 100)
		{
			MeteorMove();
			MeteorTimer = Value_Zero;
		}

		if (GameStart == true)
		{
			UpdateEnemyPopCommands();
		}

		//�G�̍X�V
		//�ʏ�G
		for (std::unique_ptr<Enemy>& enemy : enemys)
		{
			if (enemy->GetDeathFlag() == false)
			{
				EnemyPos = enemy->GetPosition();
				enemy->Update();

				EnemyFire = enemy->GetFireTime();

				EnemyFire--;

				enemy->SetFireTime(EnemyFire);

				if (enemy->GetFireTime() <= Value_Zero)
				{
					EnemyAttack(EnemyPos);

					//���˃^�C�}�[��������
					enemy->SetFireTime(enemy->GetIntervalTime());
				}

				if (enemy->GetDownFlag() == true)
				{
					particle->LevelUpParticle(5, EnemyPos, player->GetPosition(),2.0f, 30, 3.0f, Value_Zero, enemy->GetEnemyDeathPartColor());
				}

				if (enemy->GetPosition().y < -50)
				{
					enemy->SetDeathFlag(true);
				}
			}

		}

		//�u���b�N�G�X�V
		for (std::unique_ptr<Enemy>& defenceenemy : Defenceenemys)
		{
			if (defenceenemy->GetDeathFlag() == false)
			{
				//���W���擾
				DefenceEnemyPos = defenceenemy->GetPosition();
				//�X�V
				defenceenemy->DefenceEnemyUpdate();

				//��ʊO�ɏo�������
				if (defenceenemy->GetPosition().x > 100)
				{
					defenceenemy->SetDeathFlag(true);
				}

				if (defenceenemy->GetPosition().x < -100)
				{
					defenceenemy->SetDeathFlag(true);
				}

				if(defenceenemy->GetPosition().z < -100)
				{
					defenceenemy->SetDeathFlag(true);
				}
			}
		}

		//�{�X�̍X�V
		if (Boss)
		{
			//�{�X�̈ʒu�擾
			BossPos = Boss->GetPosition();
			BossCore->SetPosition(BossPos);
			BossCore->Update();

			//�{�X�R�A��
			PartsPos_U.x = BossPos.x;
			PartsPos_U.y = BossPos.y;
			PartsPos_U.z = BossPos.z;

			//�{�X�R�A�E
			PartsPos_R.x = BossPos.x;
			PartsPos_R.y = BossPos.y;
			PartsPos_R.z = BossPos.z;

			//�{�X�R�A��
			PartsPos_D.x = BossPos.x;
			PartsPos_D.y = BossPos.y;
			PartsPos_D.z = BossPos.z;

			//�{�X�R�A��
			PartsPos_L.x = BossPos.x;
			PartsPos_L.y = BossPos.y;
			PartsPos_L.z = BossPos.z;

			//�~�^����
			PartsPos_U.y = cos(TestVal * 0.05f) * 20 + Boss->GetPosition().y;
			PartsPos_U.x = sin(TestVal * 0.05f) * 20 + Boss->GetPosition().x;
			BossParts_U->SetPosition(PartsPos_U);

			//�~�^���E
			PartsPos_R.y = sin(TestVal * 0.05f) * 20 + Boss->GetPosition().y;
			PartsPos_R.x = -cos(TestVal * 0.05f) * 20 + Boss->GetPosition().x;
			BossParts_R->SetPosition(PartsPos_R);

			//�~�^����
			PartsPos_D.y = -cos(TestVal * 0.05f) * 20 + Boss->GetPosition().y;
			PartsPos_D.x = -sin(TestVal * 0.05f) * 20 + Boss->GetPosition().x;
			BossParts_D->SetPosition(PartsPos_D);

			//�~�^����
			PartsPos_L.y = -sin(TestVal * 0.05f) * 20 + Boss->GetPosition().y;
			PartsPos_L.x = cos(TestVal * 0.05f) * 20 + Boss->GetPosition().x;
			BossParts_L->SetPosition(PartsPos_L);

			TestVal += 1;
			Boss->Update();
			BossParts_U->Update();
			BossParts_R->Update();
			BossParts_D->Update();
			BossParts_L->Update();



			if (Boss->GetDeathFlag() == false)
			{
				if (Boss->GetDeathEffect() == false)
				{
					BossFire--;
				}

				if (BossFire <= Value_Zero)
				{
					BossAttack(Boss->GetPosition());

					//���˃^�C�}�[��������
					BossFire = Boss->GetIntervalTime();
				}

			}


			if (BossParts_U->GetHP() > 0)
			{
				

				BossPartsFire--;
				
				BossParts_U->SetFireTime(BossPartsFire);

				if (BossPartsFire <= Value_Zero)
				{
					EnemyAttack(PartsPos_U);

					//���˃^�C�}�[��������
					BossParts_U->SetFireTime(BossParts_U->GetIntervalTime());
					BossPartsFire = BossParts_U->GetIntervalTime();
				}
			}

			//�E
			if (BossParts_R->GetHP() > 0)
			{


				BossPartsFire--;

				BossParts_R->SetFireTime(BossPartsFire);

				if (BossPartsFire <= Value_Zero)
				{
					EnemyAttack(PartsPos_R);

					//���˃^�C�}�[��������
					BossParts_R->SetFireTime(BossParts_R->GetIntervalTime());
					BossPartsFire = BossParts_R->GetIntervalTime();
				}

			}

			//��
			if (BossParts_D->GetHP() > 0)
			{


				BossPartsFire--;

				BossParts_D->SetFireTime(BossPartsFire);

				if (BossPartsFire <= Value_Zero)
				{
					EnemyAttack(PartsPos_D);

					//���˃^�C�}�[��������
					BossParts_D->SetFireTime(BossParts_D->GetIntervalTime());
					BossPartsFire = BossParts_D->GetIntervalTime();
				}

			}

			//��
			if (BossParts_L->GetHP() > 0)
			{


				BossPartsFire--;

				BossParts_L->SetFireTime(BossPartsFire);

				if (BossPartsFire <= Value_Zero)
				{
					EnemyAttack(PartsPos_L);

					//���˃^�C�}�[��������
					BossParts_L->SetFireTime(BossParts_L->GetIntervalTime());
					BossPartsFire = BossParts_L->GetIntervalTime();
				}

			}

		}


		//�e�X�V
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			particle->FireParticle(2, bullet->GetPosition(), 2.0f, 30, 5.0f, Value_Zero, bullet->GetSpeed(), bullet->GetBulletColor());
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

		//�G�̒e�X�V
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
		{
			particle->EnemyFireParticle(2, bullet->GetPosition(), 2.0f, 30, 1.5f, Value_Zero, bullet->GetSpeed(), EnemyBulletColor);
			//���Ƃ��Ĉ������v���C���[�̍��W�ɂ��Ă���
			bullet->Update(PlayerPos, PlayerPos);
		}

		//�J�E���^�[�e�̍X�V
		for (std::unique_ptr<EnemyBullet>& bullet : counterbullets)
		{
			particle->EnemyFireParticle(2, bullet->GetPosition(), 2.0f, 30, 1.5f, Value_Zero, bullet->GetSpeed(), { 1.0, 0.1, 0.1, 1.0 });
			//���Ƃ��Ĉ������v���C���[�̍��W�ɂ��Ă���
			bullet->Update(PlayerPos, PlayerPos);
		}

		//�{�X�̒e�X�V
		for (std::unique_ptr<BossBullet>& bullet : bossbullets)
		{
			particle->EnemyFireParticle(2, bullet->GetPosition(), 2.0f, 30, 1.5f, Value_Zero, bullet->GetSpeed(), { 1.0, 0.1, 1.0, 1.0 });
			//���Ƃ��Ĉ������v���C���[�̍��W�ɂ��Ă���
			bullet->Update(PlayerPos, PlayerPos);
		}

		//�f�X�t���O�̗������e�̍폜
		//�v���C���[�e
		bullets.remove_if([](std::unique_ptr<Bullet>& bullet)
			{
				return bullet->GetDeathFlag();
			});

		//�R�A�e
		CoreR_bullets.remove_if([](std::unique_ptr<Bullet>& CoreR_bullet)
			{
				return CoreR_bullet->GetDeathFlag();
			});

		CoreL_bullets.remove_if([](std::unique_ptr<Bullet>& CoreL_bullet)
			{
				return CoreL_bullet->GetDeathFlag();
			});

		//�G�e
		enemybullets.remove_if([](std::unique_ptr<EnemyBullet>& Enemybullet)
			{
				return Enemybullet->GetDeathFlag();
			});

		counterbullets.remove_if([](std::unique_ptr<EnemyBullet>& Enemybullet)
			{
				return Enemybullet->GetDeathFlag();
			});

		//�{�X�e
		bossbullets.remove_if([](std::unique_ptr<BossBullet>& Bossbullet)
			{
				return Bossbullet->DeathGetter();
			});

		//覐�
		objects.remove_if([](std::unique_ptr<DefenceObject>& defenceObject)
			{
				return defenceObject->GetDeathFlag();
			});

		//�I�u�W�F�N�g
		for (std::unique_ptr<DefenceObject>& object : objects)
		{
			float speed = 50.0f;

			//���˃^�C�}�[��������
			for (std::unique_ptr<DefenceObject>& object : objects)
			{
				if (object->GetPosition().z < -10)
				{
					object->SetDeathFlag(true);
				}
			}

			object->Update(speed);
		}


		//�����蔻��
		//�v���C���[�̒e�ƓG�̓����蔻��
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				EnemyPos = enemy->GetPosition();
				//�����蔻��m�F
				if (enemy->GetDeathFlag() == false)
				{
					if (CheckCollision(bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//�p�[�e�B�N���𐶐�
						particle->CreateParticleInfo(50, EnemyPos, 2.0f, 30, 10.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						bullet->SetDeathFlag(true);
						EnemyDown(EnemyPos);
						enemy->SetDownFlag(true);
						player->EXP++;
					}
				}
			}
		}

		//�R�A�ƓG�̓����蔻��
		//�E
		for (std::unique_ptr<Bullet>& CoreR_bullet : CoreR_bullets)
		{
			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				EnemyPos = enemy->GetPosition();
				//�����蔻��m�F
				if (enemy->GetDeathFlag() == false)
				{
					if (CheckCollision(CoreR_bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//�p�[�e�B�N���𐶐�
						particle->CreateParticleInfo(50, EnemyPos, 2.0f, 30, 2.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						CoreR_bullet->SetDeathFlag(true);
						enemy->SetDeathFlag(true);
						player->EXP++;
					}
				}
			}
		}

		//��
		for (std::unique_ptr<Bullet>& CoreL_bullet : CoreL_bullets)
		{
			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				EnemyPos = enemy->GetPosition();
				//�����蔻��m�F
				if (enemy->GetDeathFlag() == false)
				{
					if (CheckCollision(CoreL_bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//�p�[�e�B�N���𐶐�
						particle->CreateParticleInfo(50, EnemyPos, 2.0f, 30, 2.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						CoreL_bullet->SetDeathFlag(true);
						enemy->SetDeathFlag(true);
						player->EXP++;
					}
				}
			}
		}

		//�v���C���[�e�Ɖ�]�G�̓����蔻��
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			for (std::unique_ptr<Enemy>& defenceenemy : Defenceenemys)
			{
				EnemyPos = defenceenemy->GetPosition();
				//�����蔻��m�F
				if (defenceenemy->GetDeathFlag() == false)
				{
					if (CheckCollision(bullet->GetPosition(), defenceenemy->GetPosition(), 2.0f, 3.0f) == true)
					{
						Audio::GetInstance()->PlayWave("Reflect.wav", Reflect_Volume, false);
						bullet->SetBulletColor(bullet->GetCounterColor());
						bullet->SetSpeed({ 0.0f, 0.0f, -5.0f });
					}
				}
			}
		}

		//�v���C���[�e(�J�E���^�[�e)�ƃv���C���[�̓����蔻��
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			//���@�Ǝ��@�̒e�����蔻��m�F
			if (CheckCollision(PlayerPos, bullet->GetPosition(), 2.0f, 2.0f) == true)
			{
				HitFlag = true;
				DamageEffectflag = true;

				//�p�[�e�B�N���𐶐�
				particle->CreateParticleInfo(10, PlayerPos, 2.0f, 30, 2.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
				//�_���[�WSE���Đ�
				Audio::GetInstance()->PlayWave("Damage.wav", 0.1f, false);
				//�v���C���[��HP���f�N�������g���Ēe�̃f�X�t���O���グ��
				player->HP--;
				bullet->SetDeathFlag(true);
			}
		}


		//�G�e�ƃv���C���[
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
		{
			//���@�ƓG�̒e�����蔻��m�F
			if (CheckCollision(PlayerPos, bullet->GetPosition(), 2.0f, 2.0f) == true)
			{
				HitFlag = true;
				DamageEffectflag = true;

				//�p�[�e�B�N���𐶐�
				particle->CreateParticleInfo(10, PlayerPos, 2.0f, 30, 2.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
				//�_���[�WSE���Đ�
				Audio::GetInstance()->PlayWave("Damage.wav", 0.1f, false);
				//�v���C���[��HP���f�N�������g���ēG�̒e�̃f�X�t���O���グ��
				player->HP-- ;
				bullet->SetDeathFlag(true);
			}
		}

		


		//�v���C���[�̒e�ƃ{�X�̓����蔻��
		if (Boss)
		{
			if (Boss->GetHP() < 30)
			{
				BossParts_U->SetIntervalTime(BossParts_U->GetLastIntervalTime());
				BossParts_R->SetIntervalTime(BossParts_R->GetLastIntervalTime());
				BossParts_D->SetIntervalTime(BossParts_D->GetLastIntervalTime());
				BossParts_L->SetIntervalTime(BossParts_L->GetLastIntervalTime());

				RandTime++;


				if (RandTime >= 60)
				{
					EnemyBulletColor.x = (float)rand() / RAND_MAX;
					EnemyBulletColor.y = (float)rand() / RAND_MAX;
					EnemyBulletColor.z = (float)rand() / RAND_MAX;

					RandTime = 0;
				}
			}

			if (Boss->GetDeathFlag() == false)
			{
				for (std::unique_ptr<Bullet>& bullet : bullets)
				{
					if (CheckCollision(bullet->GetPosition(), Boss->GetPosition(), 7.0f, 7.0f) == true)
					{
						//�p�[�e�B�N���𐶐�
						particle->CreateParticleInfo(10, Boss->GetPosition(), 2.0f, 50, 3.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
						//�{�X�̃J���[��ύX
						Boss->SetDamegeFlag(true);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						Boss->SetHP(Boss->GetHP() - 1);
						if (Boss->GetHP() <= Value_Zero)
						{
							Audio::GetInstance()->PlayWave("BossDown.wav", 0.1f, false);
						}
						bullet->SetDeathFlag(true);
					}


					//�v���C���[�̒e�ƃ{�X�R�A�̓����蔻��(��)
					if (BossParts_U->GetHP() > 0 && Boss->GetHP() > 0)
					{
						if (BossParts_U->GetFireTime() <= Value_Zero)
						{
							EnemyAttack(PartsPos_U);
						}

						if (CheckCollision(bullet->GetPosition(), PartsPos_U, 3.0f, 3.0f) == true)
						{
							//�p�[�e�B�N���𐶐�
							particle->CreateParticleInfo(10, PartsPos_U, 2.0f, 50, 3.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
							Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
							BossParts_U->SetHP(BossParts_U->GetHP() - 1);
							bullet->SetDeathFlag(true);
							//HP��0�ɂȂ�����p�[�e�B�N���𔭐������ď���
							if (Boss->GetHP() <= Value_Zero)
							{
								particle->CreateParticleInfo(30, PartsPos_U, 2.0f, 30, 6.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
							}
						}
					}
					

					//�v���C���[�̒e�ƃ{�X�R�A�̓����蔻��(�E)
					if (BossParts_R->GetHP() > 0 && Boss->GetHP() > 0)
					{
						if (BossParts_R->GetFireTime() <= Value_Zero)
						{
							EnemyAttack(PartsPos_U);
						}

						if (CheckCollision(bullet->GetPosition(), PartsPos_U, 3.0f, 3.0f) == true)
						{
							//�p�[�e�B�N���𐶐�
							particle->CreateParticleInfo(10, PartsPos_R, 2.0f, 50, 3.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
							Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
							BossParts_R->SetHP(BossParts_R->GetHP() - 1);
							bullet->SetDeathFlag(true);
							//HP��0�ɂȂ�����p�[�e�B�N���𔭐������ď���
							if (Boss->GetHP() <= Value_Zero)
							{
								particle->CreateParticleInfo(30, PartsPos_U, 2.0f, 30, 6.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
							}
						}
					}


					//�v���C���[�̒e�ƃ{�X�R�A�̓����蔻��(��)
					if (BossParts_D->GetHP() > 0 && Boss->GetHP() > 0)
					{
						if (BossParts_D->GetFireTime() <= Value_Zero)
						{
							EnemyAttack(PartsPos_D);
						}

						if (CheckCollision(bullet->GetPosition(), PartsPos_D, 3.0f, 3.0f) == true)
						{
							//�p�[�e�B�N���𐶐�
							particle->CreateParticleInfo(10, PartsPos_D, 2.0f, 50, 3.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
							Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
							BossParts_D->SetHP(BossParts_D->GetHP() - 1);
							bullet->SetDeathFlag(true);
							//HP��0�ɂȂ�����p�[�e�B�N���𔭐������ď���
							if (Boss->GetHP() <= Value_Zero)
							{
								particle->CreateParticleInfo(30, PartsPos_D, 2.0f, 30, 6.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
							}
						}
					}


					//�v���C���[�̒e�ƃ{�X�R�A�̓����蔻��(��)
					if (BossParts_L->GetHP() > 0 && Boss->GetHP() > 0)
					{
						if (BossParts_L->GetFireTime() <= Value_Zero)
						{
							EnemyAttack(PartsPos_L);
						}

						if (CheckCollision(bullet->GetPosition(), PartsPos_L, 3.0f, 3.0f) == true)
						{
							//�p�[�e�B�N���𐶐�
							particle->CreateParticleInfo(10, PartsPos_L, 2.0f, 50, 3.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
							Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
							BossParts_L->SetHP(BossParts_L->GetHP() - 1);
							bullet->SetDeathFlag(true);
							//HP��0�ɂȂ�����p�[�e�B�N���𔭐������ď���
							if (Boss->GetHP() <= Value_Zero)
							{
								particle->CreateParticleInfo(30, PartsPos_D, 2.0f, 30, 6.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
							}
						}
					}
				}

			}

			if (Boss->GetHP() <= Value_Zero)
			{
				BossPartTimer++;

				if (BossPartTimer >= 20 && Boss->GetDeathFlag() == false)
				{
					BossPartTimer = Value_Zero;
					PartCount++;
					particle->CreateParticleInfo(10, Boss->GetPosition(), 2.0f, 50, 8.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
				}

				if (PartCount >= 5)
				{
					particle->CreateParticleInfo(60, Boss->GetPosition(), 4.0f, 50, 15.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
					PartCount = Value_Zero;
					Boss->SetDeathFlag(true);
				}

			}
		}

		//�{�X�̒e�ƃv���C���[�̓����蔻��
		if (Boss)
		{
			for (std::unique_ptr<BossBullet>& bullet : bossbullets)
			{
				//���@�ƓG�̒e�����蔻��m�F
				if (CheckCollision(PlayerPos, bullet->GetPosition(), 2.0f, 2.0f) == true)
				{
					HitFlag = true;
					DamageEffectflag = true;
					//�p�[�e�B�N���𐶐�
					particle->CreateParticleInfo(10, PlayerPos, 2.0f, 30, 2.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });

					if (player->HP == 1)
					{
						
						particle->CreateParticleInfo(50, PlayerPos, 2.0f, 50, 5.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
						Audio::GetInstance()->PlayWave("BossDown.wav", 0.1f, false);
					}
					player->HP--;
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
				ShakeTimer = Value_Zero;
				camera->SetEye(CameraDefault);
			}
		}

		if (DamageEffectflag == true)
		{
			Damage();
		}
		//�V���X�V
		CelestialSphere->Update();

		MoonRot.y += 0.2f;

		Moon->SetRotation(MoonRot);

		MarsRot.y -= 0.4f;

		Mars->SetRotation(MarsRot);


		NeptuneRot.y -= 0.4f;

		Neptune->SetRotation(MarsRot);


		Moon->Update();

		Mars->Update();

		Neptune->Update();


		Station->SetRotation(StationRot);
		
		Station->Update();
		Shooting->Update();
		Force->Update();

		CameraObject->Update();


		//�X�v���C�g�̍X�V

		UpdateSprite();

		//�J�����̍X�V
		camera->Update();

		//�p�[�e�B�N���X�V
		particle->Update();
		particle_Red->Update();

		UpdateInput();


	//}

	//Esc�L�[�ŃE�B���h�E�����
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))	//ESC�L�[�ŃE�B���h�E�����
	{
		//endRequest_ = true;
		return;
	}
}

void GameScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//////�X�v���C�g�̋��ʃR�}���h
	//SpriteCommon::GetInstance()->PreDraw()

	////�X�v���C�g�`��



	/*SpriteCommon::PostDraw();*/
	
	//3D�I�u�W�F�N�g�̕`��O����
	Object3d::PreDraw();
	
	

	if (Boss)
	{

		if (Boss->GetDeathFlag() == false)
		{
			Boss->Draw();
			
			if (BossParts_U->GetHP() > 0)
			{
				BossParts_U->Draw();
			}

			if (BossParts_R->GetHP() > 0)
			{
				BossParts_R->Draw();
			}

			if (BossParts_D->GetHP() > 0)
			{
				BossParts_D->Draw();
			}

			if (BossParts_L->GetHP() > 0)
			{
				BossParts_L->Draw();
			}
		}

		//�{�X�o��^�C�}�[�̃f�N�������g
		if (BossTimer > Value_Zero)
		{
			BossTimer--;
		}
	}

	//�G�̕`��
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (enemy->GetDeathFlag() == false)
		{
			enemy->Draw();
		}
	}

	for (std::unique_ptr<Enemy>& defenceenemy : Defenceenemys)
	{
		if (defenceenemy->GetDeathFlag() == false)
		{
			defenceenemy->Draw();
		}
	}



	//�{�X�̒e�`��
	for (std::unique_ptr<DefenceObject>& object : objects)
	{
		object->Draw();
	}

	//�V���̕`��
	CelestialSphere->Draw();

	Moon->Draw();
	Mars->Draw();
	Neptune->Draw();
	Station->Draw();
	Shooting->Draw();
	Force->Draw();

	if (player->Level >= 2)
	{
		Satellite_R->Draw();
	}

	if (player->Level >= 3)
	{
		Satellite_L->Draw();
	}

	player->Draw();

	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();

	Particle::PreDraw();

	
	particle->Draw();

	particle_Red->Draw();

	

	Particle::PostDraw();


	////�X�v���C�g�̋��ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();

	//DrawDebugText();

	Stage_1->Draw();
	Go->Draw();
	LoadBG->Draw();

	DrawSprite();


}

void GameScene::EnemyInit()
{
	//���˃^�C�}�[��������
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->SetFireTime(enemy->GetIntervalTime());
	}
}

void GameScene::BossInit()
{
	Boss->SetFireTime(Boss->GetIntervalTime());

	BossParts_U->SetFireTime(BossParts_U->GetIntervalTime());
	BossParts_R->SetFireTime(BossParts_R->GetIntervalTime());
	BossParts_D->SetFireTime(BossParts_D->GetIntervalTime());
	BossParts_L->SetFireTime(BossParts_L->GetIntervalTime());
}

void GameScene::EnemyUpdate(XMFLOAT3 enemyPos)
{
	EnemyBulletTimer--;
	
}

void GameScene::Attack(XMFLOAT3 StartPos)
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) || IsButtonPush(ButtonKind::Button_B))
	{
		//�e�𐶐���������
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet = Bullet::Create(model_Bullet, camera, StartPos, ReticlePos/*Reticle->position_*/);

		//�e��o�^
		bullets.push_back(std::move(newBullet));
	}
}

void GameScene::EnemyAttack(XMFLOAT3 EnemyPos)
{
	
		//�e�𐶐���������
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet = EnemyBullet::Create(model_EnemyBullet, camera, EnemyPos,PlayerPos);

		//�e��o�^
		enemybullets.push_back(std::move(newBullet));
}

void GameScene::CounterAttack(XMFLOAT3 EnemyPos)
{
	//�e�𐶐���������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet = EnemyBullet::Create(model_EnemyBullet, camera, EnemyPos, PlayerPos);

	//�e��o�^
	counterbullets.push_back(std::move(newBullet));
}

void GameScene::BossAttack(XMFLOAT3 BossPos)
{
	//�e�𐶐���������
	std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
	newBullet = BossBullet::Create(model_EnemyBullet, camera, BossPos, PlayerPos);

	bossbullets.push_back(std::move(newBullet));
}

void GameScene::LoadEnemyPopData()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open(L"Resources/EnemyPop.csv");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	//�ҋ@����
	if (EnemyFlag)
	{
		EnemyTimer--;

		if (EnemyTimer <= Value_Zero)
		{
			//�ҋ@����
			EnemyFlag = false;
		}
		return;
	}

	//1�����̕����������ϐ�
	std::string line;

	//�R�}���h���s���[�v
	while (getline(enemyPopCommands, line))
	{
		//1�����̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;

		//,��؂�ł̍s�̐擪��������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g�A�E�g
		if (word.find("//") == Value_Zero)
		{
			//�R�����g���΂�
			continue;
		}

		//POP�R�}���h
		if (word.find("POP") == Value_Zero)
		{
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());


			//�擾����x,y,z���W���i�[
			XMFLOAT3 EnemyPos = { x, y, z };

			//�G�𔭐�������
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy = Enemy::Create(model_Enemy, camera, EnemyPos);

			enemys.push_back(std::move(newEnemy));

		}

		//POP�R�}���h
		if (word.find("DEFENCEPOP") == Value_Zero)
		{
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());


			//�擾����x,y,z���W���i�[
			XMFLOAT3 EnemyPos = { x, y, z };

			//�G�𔭐�������
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy = Enemy::Create(model_DefenceEnemy, camera, EnemyPos);

			Defenceenemys.push_back(std::move(newEnemy));

		}

		//POP�R�}���h
		else if (word.find("BOSS_POP") == Value_Zero)
		{
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());


			//�擾����x,y,z���W���i�[
			XMFLOAT3 EnemyPos = { x, y, z };

			//�G�𔭐�������
			Boss = Boss::Create(model_Boss, camera, EnemyPos);
			BossCore = Object3d::Create(model_reticle, camera);
			BossParts_U = BossParts::Create(model_BossCore, camera, {0.0f , 0.0f , 0.0f});
			BossParts_R = BossParts::Create(model_BossCore, camera, { 0.0f , 0.0f , 0.0f });
			BossParts_D = BossParts::Create(model_BossCore, camera, { 0.0f , 0.0f , 0.0f });
			BossParts_L = BossParts::Create(model_BossCore, camera, { 0.0f , 0.0f , 0.0f });
		}

		//POP�R�}���h
		else if (word.find("OBJECT_POP") == Value_Zero)
		{
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());


			//�擾����x,y,z���W���i�[
			XMFLOAT3 EnemyPos = { x, y, z };

			//�G�𔭐�������
			std::unique_ptr<DefenceObject> newObject = std::make_unique<DefenceObject>();
			newObject = DefenceObject::Create(model_Meteor, camera, EnemyPos);

			objects.push_back(std::move(newObject));
		}

		//WAIT�R�}���h
		else if (word.find("WAIT") == Value_Zero)
		{
			getline(line_stream, word, ',');

			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�ҋ@����
			EnemyFlag = true;
			EnemyTimer = waitTime;

			//�R�}���h���[�v�𔲂���
			break;
		}

		else if (word.find("EVENT_S") == Value_Zero)
		{
			getline(line_stream, word, ',');

			BossFlag_S = true;
		}

		else if (word.find("EVENT_E") == Value_Zero)
		{
			getline(line_stream, word, ',');

			BossFlag_E = true;
			BossFlag_S = false;
		}

		else if (word.find("UI_FLAGDOWN") == Value_Zero)
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
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(43, L"Resources/Image/HPBar.png");

	sprite = Sprite::Create(1, { 1, 1, 1, 1 }, { 0, 0 }, false, false);
	UI = Sprite::Create(2, { 1, 1, 1, 1 }, { 0.5, 0.5 }, false, false);
	
	
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

	//�^�C�g��UI
	StartUI = Sprite::Create(42, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	StartUI->SetSize({ 100,100 });
	StartUI->SetPosition({ 500, 500, 0 });

	//HP�o�[
	HPBar = Sprite::Create(43, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	HPBar->SetSize(PlayerHPSize);
	HPBar->TransferVertexBuffer();
	HPBar->SetPosition({ -50, 800, 0 });
	HPBar->SetColor({ 0.0f, 1.0f,0.498f , 1});

	EmpBar = Sprite::Create(43, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	EmpBar->SetSize(PlayerHPSize);
	EmpBar->TransferVertexBuffer();
	EmpBar->SetPosition({ -50, 800, 0 });
	EmpBar->SetColor({ 0.501f, 0.501f,0.501f , 1 });

	PlayerFrame = Sprite::Create(43, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	PlayerFrame->SetSize(FrameSize);
	PlayerFrame->TransferVertexBuffer();
	PlayerFrame->SetPosition({ -50, 800, 0 });
	PlayerFrame->SetColor({ 0.247f, 0.349f,0.211f , 1 });

	EmpBossBar = Sprite::Create(43, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	EmpBossBar->SetSize(BossHPSize);
	EmpBossBar->TransferVertexBuffer();
	EmpBossBar->SetPosition({ 220, -100, 0 });
	EmpBossBar->SetColor({ 0.501f, 0.501f,0.501f , 1 });

	BossFrame = Sprite::Create(43, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	BossFrame->SetSize(FrameSize);
	BossFrame->TransferVertexBuffer();
	BossFrame->SetPosition({ 220, -100, 0 });
	BossFrame->SetColor({ 0.247f, 0.349f,0.211f , 1 });

	//�{�XHP�o�[
	BossHPBar = Sprite::Create(43, { 1, 1, 1, 1.0 }, { 0, 0 }, false, false);
	BossHPBar->SetSize(BossHPSize);
	BossHPBar->TransferVertexBuffer();
	BossHPBar->SetPosition({ 220, -100, 0 });
	BossHPBar->SetColor({ 1.0f, 0.207f,0.0f , 1 });
}

void GameScene::UpdateSprite()
{
	sprite->Update();
	

	//�o���lUI
	if (PlayerPos.x > 35)
	{
		if (ExpBar_0->color_.w > MIN_ALPHA)
		{
			ExpBar_0->color_.w -= 0.1f;
		}

		if (ExpBar_1->color_.w > MIN_ALPHA)
		{
			ExpBar_1->color_.w -= 0.1f;
		}

		if (ExpBar_2->color_.w > MIN_ALPHA)
		{
			ExpBar_2->color_.w -= 0.1f;
		}

		if (ExpBar_3->color_.w > MIN_ALPHA)
		{
			ExpBar_3->color_.w -= 0.1f;
		}

		if (ExpBar_4->color_.w > MIN_ALPHA)
		{
			ExpBar_4->color_.w -= 0.1f;
		}

		if (ExpBar_5->color_.w > MIN_ALPHA)
		{
			ExpBar_5->color_.w -= 0.1f;
		}

		if (Level_1->color_.w > MIN_ALPHA)
		{
			Level_1->color_.w -= 0.1f;
		}

		if (Level_2->color_.w > MIN_ALPHA)
		{
			Level_2->color_.w -= 0.1f;
		}

		if (Level_3->color_.w > MIN_ALPHA)
		{
			Level_3->color_.w -= 0.1f;
		}
		
	}

	else
	{
		ExpBar_0->color_.w = MAX_ALPHA;
		ExpBar_1->color_.w = MAX_ALPHA;
		ExpBar_2->color_.w = MAX_ALPHA;
		ExpBar_3->color_.w = MAX_ALPHA;
		ExpBar_4->color_.w = MAX_ALPHA;
		ExpBar_5->color_.w = MAX_ALPHA;
		Level_1->color_.w = MAX_ALPHA;
		Level_2->color_.w = MAX_ALPHA;
		Level_3->color_.w = MAX_ALPHA;
	}
	ExpBar_0->Update();
	ExpBar_1->Update();
	ExpBar_2->Update();
	ExpBar_3->Update();
	ExpBar_4->Update();
	ExpBar_5->Update();
	Level_1->Update();
	Level_2->Update();
	Level_3->Update();

	Stage_1->Update();
	Go->Update();
	BossUI_U->Update();
	BossUI_D->Update();
	BossUI_U_2->Update();
	BossUI_D_2->Update();
	Warning->Update();
	DamageEffect->Update();

	HPdiv = player->GetHP() / player->GetMAXHP();
	PlayerNowHP.x = HPdiv * PlayerHPSize.x;

	if (player->GetHP() >= Value_Zero)
	{
		HPBar->SetSize(PlayerNowHP);
	}

	if (player->GetHP() > 3 && player->GetHP() <= 5)
	{
		particle->WarningParticle(1, { PlayerPos.x, PlayerPos.y , PlayerPos.z - 12 }, PartVel, 10, 3.0f, Value_Zero, WarningPartSpeed, { 0.901, 0.0, 0.200, 1.0 });
		particle->WarningParticle(1, { PlayerPos.x, PlayerPos.y , PlayerPos.z - 12 }, PartVel, 10, 3.0f, Value_Zero, WarningPartSpeed, { 1.0, 0.615, 0.200, 1.0 });
		HPBar->SetColor({ 0.964f, 0.921f, 0.415f , 1 });
	}

	if (player->GetHP() > 0 && player->GetHP() <= 3)
	{
		particle->WarningParticle(2, { PlayerPos.x, PlayerPos.y , PlayerPos.z - 12 }, PartVel, 15, 3.0f, Value_Zero, WarningPartSpeed, { 0.901, 0.0, 0.200, 1.0 });
		particle->WarningParticle(2, { PlayerPos.x, PlayerPos.y , PlayerPos.z - 12 }, PartVel, 15, 3.0f, Value_Zero, WarningPartSpeed, { 1.0, 0.615, 0.200, 1.0 });
		HPBar->SetColor({ 0.854f, 0.101f, 0.062f , 1 });
	}

	HPBar->TransferVertexBuffer();

	HPBar->Update();
	EmpBar->Update();
	PlayerFrame->Update();
	BossFrame->Update();

	//�{�XHP
	if (Boss)
	{
		BossHPdiv = Boss->GetHP() / Boss->GetMAXHP();
		BossNowHP.x = BossHPdiv * BossHPSize.x;
		if (Boss->GetHP() >= Value_Zero)
		{
			BossHPBar->SetSize(BossNowHP);
		}
		BossHPBar->TransferVertexBuffer();
		BossHPBar->Update();
		EmpBossBar->Update();
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

	//���[��UI
	if (PlayerPos.x < -30 && PlayerPos.y > 15)
	{
		if (Rule->color_.w > MIN_ALPHA)
		{
			Rule->color_.w -= 0.1f;
		}
	}

	else
	{
		Rule->color_.w = MAX_ALPHA;
	}

	Rule->Update();
}

void GameScene::DrawSprite()
{
	if (DamageEffectflag == true)
	{
		DamageEffect->Draw();
	}

	
	//�o���lUI�̕`��

	if (Go->ComFlag == true)
	{
		if (player->EXP == Value_Zero)
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

		//���x��
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

		//���[��UI
		//Rule->Draw();
	}

	//�{�X���oUI
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

	//�^�C�g��UI
	if (GameStart == false)
	{
		StartUI->Draw();
	}

	PlayerFrame->Draw();
	BossFrame->Draw();
	EmpBar->Draw();
	HPBar->Draw();
	if (Boss)
	{
		EmpBossBar->Draw();
		BossHPBar->Draw();
	}
}

void GameScene::Start()
{
	if (PlayerPos.z < Value_Zero)
	{
		player->MoveCanFlag = false;
	}
	Stage_1->Vec.x = (Stage_1->PointPos.x - Stage_1->position_.x) / 20;
	Stage_1->position_.x += Stage_1->Vec.x;
	//�v���C���[�o��
	if (PlayerMoveFlag == false)
	{
		PlayerMoveVec.z = (player->GetStartPointPos().z - PlayerPos.z) / 20;
		PlayerPos.z += PlayerMoveVec.z;
	}

	if (Stage_1->position_.x <= Stage_1->PointPos.x + 10)
	{
		Stage_1->PointPos.x = -400;
		Stage_1->PointPos.y = 285;

		
	}

	if (Stage_1->position_.x <= Stage_1->PointPos.x + 10)
	{

		Go->Vec.y = (Go->PointPos.y - Go->position_.y) / 40;
		Go->position_.y += Go->Vec.y;
		Go->ComFlag = true;

		player->SetStartPointPos(player->GetFirstPos());
	}

	if (Go->ComFlag == true)
	{
		if (PlayerPos.z < Value_Zero)
		{
	

			DelayTime++;
		}

		if (DelayTime >= 5)
		{
			StartCamMove = true;
		}

		if (StartCamMove == true)
		{
			MoveCamVec.z = (PointCameraPos.z - CameraPos.z) / 20;
			PlayerRoleVec.z = (PlayerPointRole.z - PlayerRot.z) / 15;

			if (camera->GetEye().z <= -50.0f)
			{
				PlayerMoveVec.z = (player->GetStartPointPos().z - PlayerPos.z) / 20;
				PlayerPos.z += PlayerMoveVec.z;

				player->SetPosition(PlayerPos);

				camera->CameraMoveEyeVector({ Value_Zero , Value_Zero ,MoveCamVec.z });
				PlayerRot.z += PlayerRoleVec.z;
			}

			if(camera->GetEye().z >= -51.0f)
			{
				DelayTime = Value_Zero;
				PlayerRot.z = 0.0f;
				PlayerPos.z = 0.0f;
				player->MoveCanFlag = true;
				player->SetGameStart(true);
				StartCamMove = false;
			}
		}
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
		BossPartTimer = Value_Zero;
		PartCount += 1;
		particle->CreateParticleInfo(10, Boss->GetPosition(), 2.0f, 50, 8.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
	}

	if (PartCount >= 5)
	{
		particle->CreateParticleInfo(60, Boss->GetPosition(), 4.0f, 50, 15.0f, Value_Zero, { 1.0, 0.654, 0.1, 1.0 });
		PartCount = Value_Zero;
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

void GameScene::DrawDebugText()
{
		//�v���C���[�̍��W
		std::ostringstream BossPosition;
		BossPosition << "BossPosition("
			<< std::fixed << std::setprecision(5)
			<< BossPos.x << ","
			<< BossPos.y << ","
			<< BossPos.z << ")";

		DebugText::GetInstance()->Print(BossPosition.str(), 0, 0, 2.0f);

			std::ostringstream PartsNowstr;
			PartsNowstr << "PartsPos("
				<< std::fixed << std::setprecision(5)
				<< PartsPos_U.x << ","
				<< PartsPos_U.y << ","
				<< PartsPos_U.z << ")";

			DebugText::GetInstance()->Print(PartsNowstr.str(), 0, 50, 2.0f);

			////Old
			std::ostringstream BossPartsFirestr;
			BossPartsFirestr << "BossPartsFire("
				<< std::fixed << std::setprecision(5)
				<< BossPartsFire << ")";

			DebugText::GetInstance()->Print(BossPartsFirestr.str(), 0, 100, 2.0f);

			////delay
			//std::ostringstream delaystr;
			//delaystr << "delaytime("
			//	<< std::fixed << std::setprecision(5)
			//	<< DelayTime << ")";

			//DebugText::GetInstance()->Print(delaystr.str(), 0, 150, 2.0f);
}

void GameScene::HpBarMove()
{
	//�v���C���[�t���[��
	PlayerFrame->PointPos.x = 40;
	PlayerFrame->PointPos.y = 640;

	PlayerFrame->Vec.x = (PlayerFrame->PointPos.x - PlayerFrame->position_.x) / 5;
	PlayerFrame->position_.x += PlayerFrame->Vec.x;
	PlayerFrame->Vec.y = (PlayerFrame->PointPos.y - PlayerFrame->position_.y) / 5;
	PlayerFrame->position_.y += PlayerFrame->Vec.y;


	//HP
	HPBar->PointPos.x = 50; 
	HPBar->PointPos.y = 650;

	HPBar->Vec.x = (HPBar->PointPos.x - HPBar->position_.x) / 5;
	HPBar->position_.x += HPBar->Vec.x;
	HPBar->Vec.y = (HPBar->PointPos.y - HPBar->position_.y) / 5;
	HPBar->position_.y += HPBar->Vec.y;


	//���HP�o�[
	EmpBar->PointPos.x = 50;
	EmpBar->PointPos.y = 650;

	EmpBar->Vec.x = (EmpBar->PointPos.x - EmpBar->position_.x) / 5;
	EmpBar->position_.x += EmpBar->Vec.x;
	EmpBar->Vec.y = (EmpBar->PointPos.y - EmpBar->position_.y) / 5;
	EmpBar->position_.y += EmpBar->Vec.y;

	//�{�X��HP�o�[
	if (Boss)
	{
		//�{�X�t���[��
		BossFrame->PointPos.x = 220;
		BossFrame->PointPos.y = 20;

		BossFrame->Vec.x = (BossFrame->PointPos.x - BossFrame->position_.x) / 5;
		BossFrame->position_.x += BossFrame->Vec.x;
		BossFrame->Vec.y = (BossFrame->PointPos.y - BossFrame->position_.y) / 5;
		BossFrame->position_.y += BossFrame->Vec.y;

		//�{�XHP
		BossHPBar->PointPos.x = 220;
		BossHPBar->PointPos.y = 30;

		BossHPBar->Vec.x = (BossHPBar->PointPos.x - BossHPBar->position_.x) / 5;
		BossHPBar->position_.x += BossHPBar->Vec.x;
		BossHPBar->Vec.y = (BossHPBar->PointPos.y - BossHPBar->position_.y) / 5;
		BossHPBar->position_.y += BossHPBar->Vec.y;

		//��
		EmpBossBar->PointPos.x = 220;
		EmpBossBar->PointPos.y = 30;

		EmpBossBar->Vec.x = (EmpBossBar->PointPos.x - EmpBossBar->position_.x) / 5;
		EmpBossBar->position_.x += EmpBossBar->Vec.x;
		EmpBossBar->Vec.y = (EmpBossBar->PointPos.y - EmpBossBar->position_.y) / 5;
		EmpBossBar->position_.y += EmpBossBar->Vec.y;
	}
}

void GameScene::EnemyDown(XMFLOAT3 EnemyPos)
{
	VY += gravity;

	EnemyPos.y += VY * 0.2f;

	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (EnemyPos.y <= -10)
		{
			//enemy->SetDeathFlag(true);
		}
	}
}

void GameScene::CameraMove()
{
	

	if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S))
	{

		PartVel = 5.0f;

		if (Input::GetInstance()->PushKey(DIK_A))
		{
			PointCameraPos.x = -CameraLimit;
			MoveCamVec.x = (PointCameraPos.x - CameraPos.x) / MoveRate;

			if (camera->GetEye().x > -CameraLimit)
			{
				camera->CameraMoveEyeVector({ MoveCamVec.x, 0, 0 });
			}
		}

		if (Input::GetInstance()->PushKey(DIK_D))
		{
			PointCameraPos.x = CameraLimit;
			MoveCamVec.x = (PointCameraPos.x - CameraPos.x) / MoveRate;

			if (camera->GetEye().x < CameraLimit)
			{
				camera->CameraMoveEyeVector({ MoveCamVec.x, 0, 0 });
			}
		}

		if (Input::GetInstance()->PushKey(DIK_W))
		{
			PointCameraPos.y = CameraLimit;
			MoveCamVec.y = (PointCameraPos.y - CameraPos.y) / MoveRate;

			if (camera->GetEye().y < CameraLimit)
			{
				camera->CameraMoveEyeVector({ 0, MoveCamVec.y, 0 });
			}
		}

		if (Input::GetInstance()->PushKey(DIK_S))
		{
			PointCameraPos.y = -CameraLimit;
			MoveCamVec.y = (PointCameraPos.y - CameraPos.y) / MoveRate;

			if (camera->GetEye().y > -CameraLimit)
			{
				camera->CameraMoveEyeVector({ 0, MoveCamVec.y, 0 });
			}
		}
	}

	else
	{
		PartVel = 10.0f;
	}
}



void GameScene::UI_AlphaIncriment(float color_w)
{
	if (color_w < MAX_ALPHA)
	{
		color_w += 0.1f;
	}
}

void GameScene::UI_AlphaDecriment(float color_w)
{
	
		color_w -= 0.1f;
}


