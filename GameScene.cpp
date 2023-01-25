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
	//�|�X�g�G�t�F�N�g�̐���
	//postEffect = PostEffect::Create(100, { 0, 0 }, false, false);

	//OBJ���烂�f���f�[�^��ǂݍ���
	//model_1 = Model::LoadFromObj("triangle_mat");
	model_2 = Model::LoadFromObj("Player");
	model_Bullet = Model::LoadFromObj("Bullet");
	model_EnemyBullet = Model::LoadFromObj("EnemyBullet");
	model_Enemy = Model::LoadFromObj("Enemy_1");
	model_Boss = Model::LoadFromObj("Boss");
	model_sphere = Model::LoadFromObj("CelestialSphere");
	model_reticle = Model::LoadFromObj("Box");

	Audio::GetInstance()->Initialize();

	Audio::GetInstance()->LoadWave("Bullet.wav");
	Audio::GetInstance()->LoadWave("BossDown.wav");
	Audio::GetInstance()->LoadWave("Damage.wav");
	Audio::GetInstance()->LoadWave("EnemyDown.wav");
	Audio::GetInstance()->LoadWave("GameOver.wav");
	Audio::GetInstance()->LoadWave("GameScene.wav");
	Audio::GetInstance()->LoadWave("GameClear.wav");
	

	//�G�̏�����
	LoadEnemyPopData();
	EnemyInit();


	//3D�I�u�W�F�N�g����
	P = Player::Create(model_2, camera);
	CoreR = Player::Create(model_reticle, camera);
	CoreL = Player::Create(model_reticle, camera);

	//�V������
	CelestialSphere = Object3d::Create(model_sphere, camera);
	CelestialSphere->SetScale({100, 100, 100});

	//���e�B�N������
	//Reticle = Reticle::Create(model_reticle, camera);
	//Reticle->SetScale({ 0.5, 0.5, 0.5 });



		//�J���������_���Z�b�g
		//camera->SetTarget({ 0, 20, 0 });


	/*object1->PlayAnimation();*/

	//�p�[�e�B�N������
	particle = Particle::Create(L"Resources/Image/effect1.png", camera);
	particle->Update();

	EnemyPart = Particle::Create(L"Resources/Image/effect3.png", camera);
	EnemyPart->Update();

	//���o�^�C�}�[������
	WaitTimer = 0;
	BossTimer = 128;

	//�X�e�[�W�X�^�[�g��UI�̏�����
	Stage_1->position_.x = 1280;
	Stage_1->position_.y = 285;
	Go->position_.y = -720;

	BossUI_U->SetColor({ 1, 1, 1, 0.7f });
	BossUI_U_2->SetColor({ 1, 1, 1, 0.7f });
	BossUI_D->SetColor({ 1, 1, 1, 0.7f });
	BossUI_D_2->SetColor({ 1, 1, 1, 0.7f });

	BossUI_U->position_.x = 0;
	BossUI_U->position_.y = 100;
	BossUI_U_2->position_.x = -1280;
	BossUI_U_2->position_.y = 100;

	BossUI_D->position_.x = 0;
	BossUI_D->position_.y = -100;
	BossUI_D_2->position_.x = 1280;
	BossUI_D_2->position_.y = -100;

	//�ŏ��̖ړI���W
	Stage_1->PointPos.x = 460;
	Stage_1->PointPos.y = 285;
	Go->PointPos.y = 0;

	BossUI_U->PointPos.x = 1280;
	BossUI_U->PointPos.y = 0;
	BossUI_U_2->PointPos.x = 0;
	BossUI_U_2->PointPos.y = 0;

	BossUI_D->PointPos.x = -1280;
	BossUI_D->PointPos.y = 0;
	BossUI_D_2->PointPos.x = 0;
	BossUI_D_2->PointPos.y = 0;

	LoadBG->color_.w = 1.0f;

	
	CoreR->SetScale({ 0.5f, 0.5f, 0.5f });
	CoreL->SetScale({ 0.5f, 0.5f, 0.5f });

	//�R���g���[���[������
	InitInput();


	Audio::GetInstance()->PlayWave("GameScene.wav", 0.1f, true);
}

void GameScene::Finalize()
{
	//�X�v���C�g���
	delete sprite;
	delete UI;
	delete HP_0;
	delete HP_1;
	delete HP_2;
	delete HP_3;
	delete HP_4;
	delete HP_5;

	//���f���̉��
	delete model_1;
	delete model_2;
}

void GameScene::Update()
{



	if (!LoadBG->color_.w <= 0.0f)
	{
		LoadBG->color_.w -= 0.02f;

		LoadBG->Update();
	}

	/*if (LoadBG->color_.w <= 0.0f)
	{
		LoadFlag = true;
	}*/

	if (LoadBG->color_.w <= 0.0f)
	{
		LoadBG->color_.w = 0.0f;
		Start();
	}

		//UI->position_.x = Reticle->MousePosition.x;
		//UI->position_.y = Reticle->MousePosition.y;

		//�v���C���[�R�A�̍��W�ݒ�
		CoreR->position_.x = P->position_.x + 6;
		CoreR->position_.y = P->position_.y - 1;
		CoreR->position_.z = P->position_.z;

		CoreL->position_.x = P->position_.x - 6;
		CoreL->position_.y = P->position_.y - 1;
		CoreL->position_.z = P->position_.z;

		CoreR->rotation_ = { 0.0f, 0.0f, 0.0f };
		CoreL->rotation_ = { 0.0f, 0.0f, 0.0f };

		if (Input::GetInstance()->PushKey(DIK_P))
		{
			UI->BarSize.x += 1;
			UI->BarSize.y += 1;

			UI->SetSize(BarSize);
		}

		//�U����SE�Đ�
		if (P->MoveCanFlag == true && Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			Audio::GetInstance()->PlayWave("Bullet.wav", 0.5f, false);
		}

		if (P->MoveCanFlag == true && IsButtonDown(ButtonKind::Button_B))
		{
			Audio::GetInstance()->PlayWave("Bullet.wav", 0.5f, false);
		}

		UI->GetSize();

		UI->Update();


		//�V�[���J��
		//�G��HP��0�ɂȂ�����Q�[���N���A
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
					Audio::GetInstance()->StopWave("GameScene.wav");
					SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
				}
			}
		}
		//�v���C���[��HP��0�ɂȂ�����Q�[���I�[�o�[
		if (P->HP <= 0)
		{
			/*if (WaitTimer < 60)
			{
				WaitTimer += 1;
			}*/
			if (WaitTimer < 60)
			{
				WaitTimer += 1;
			}

			if (WaitTimer >= 60)
			{
				WaitTimer = 0;
				Audio::GetInstance()->StopWave("GameScene.wav");
				SceneManager::GetInstance()->ChangeScene("GAMEOVER");
			}

		}


		//3D�I�u�W�F�N�g�̍X�V

		//player->Update();

		//�v���C���[�̍X�V
		P->Update();
		CoreR->Update();
		CoreL->Update();

		/*Reticle->Update();*/

		//�v���C���[�̍U���֐�
		if (P->MoveCanFlag == true)
		{
			Attack(P->position_);
		}

		if (P->Level >= 2)
		{
			Attack(CoreR->position_);
		}

		if (P->Level >= 3)
		{
			Attack(CoreL->position_);
		}

		if (Boss)
		{
			if (Boss->DeathFlag == false)
			{
				//�{�X�̍X�V
				Boss->Update();
			}
		}


		UpdateEnemyPopCommands();

		//�G�̍X�V
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

					//���˃^�C�}�[��������
					enemy->FireTime = enemy->IntervalTime;
				}
			}

		}

		//�{�X�̍X�V
		if (Boss)
		{
			if (Boss->DeathFlag == false)
			{
				Boss->Update();

				Boss->FireTime--;

				if (Boss->FireTime <= 0)
				{
					EnemyAttack(Boss->position);

					//���˃^�C�}�[��������
					Boss->FireTime = Boss->IntervalTime;
				}

			}
		}


		//�e�X�V
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			bullet->Update(P->position_);
		}

		for (std::unique_ptr<Bullet>& CoreR_bullet : CoreR_bullets)
		{
			CoreR_bullet->Update(CoreR->position_);
		}

		for (std::unique_ptr<Bullet>& CoreL_bullet : CoreL_bullets)
		{
			CoreL_bullet->Update(CoreL->position_);
		}

		//�G�̒e�X�V
		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
		{
			//���Ƃ��Ĉ������v���C���[�̍��W�ɂ��Ă���
			bullet->Update(P->position_, P->position_);
		}

		//�{�X�̒e�X�V
		for (std::unique_ptr<BossBullet>& bullet : bossbullets)
		{
			//���Ƃ��Ĉ������v���C���[�̍��W�ɂ��Ă���
			bullet->Update(P->position_, P->position_);
		}

		//�f�X�t���O�̗������e�̍폜
		//�v���C���[�e
		bullets.remove_if([](std::unique_ptr<Bullet>& bullet)
			{
				return bullet->DeathGetter();
			});

		//�R�A�e
		CoreR_bullets.remove_if([](std::unique_ptr<Bullet>& CoreR_bullet)
			{
				return CoreR_bullet->DeathGetter();
			});

		CoreL_bullets.remove_if([](std::unique_ptr<Bullet>& CoreL_bullet)
			{
				return CoreL_bullet->DeathGetter();
			});

		//�G�e
		enemybullets.remove_if([](std::unique_ptr<EnemyBullet>& Enemybullet)
			{
				return Enemybullet->DeathGetter();
			});

		//�{�X�e
		bossbullets.remove_if([](std::unique_ptr<BossBullet>& Bossbullet)
			{
				return Bossbullet->DeathGetter();
			});


		//�����蔻��
		//�v���C���[�̒e�ƓG�̓����蔻��
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				//�����蔻��m�F
				if (enemy->DeathFlag == false)
				{
					if (CheckCollision(bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//�p�[�e�B�N���𐶐�
						EnemyPart->CreateParticleInfo(50, enemy->position, 2.0f, 30, 2.0f, 0.0f);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						bullet->DeathFlag = true;
						enemy->DeathFlag = true;
						P->EXP += 1;
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
				//�����蔻��m�F
				if (enemy->DeathFlag == false)
				{
					if (CheckCollision(CoreR_bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//�p�[�e�B�N���𐶐�
						EnemyPart->CreateParticleInfo(50, enemy->position, 2.0f, 30, 2.0f, 0.0f);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						CoreR_bullet->DeathFlag = true;
						enemy->DeathFlag = true;
						P->EXP += 1;
					}
				}
			}
		}

		//��
		for (std::unique_ptr<Bullet>& CoreL_bullet : CoreL_bullets)
		{
			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				//�����蔻��m�F
				if (enemy->DeathFlag == false)
				{
					if (CheckCollision(CoreL_bullet->GetPosition(), enemy->GetPosition(), 2.0f, 2.0f) == true)
					{
						//�p�[�e�B�N���𐶐�
						EnemyPart->CreateParticleInfo(50, enemy->position, 2.0f, 30, 2.0f, 0.0f);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						CoreL_bullet->DeathFlag = true;
						enemy->DeathFlag = true;
						P->EXP += 1;
					}
				}
			}
		}

		for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
		{
			//���@�ƓG�̒e�����蔻��m�F
			if (CheckCollision(P->GetPosition(), bullet->GetPosition(), 2.0f, 2.0f) == true)
			{
				//�p�[�e�B�N���𐶐�
				particle->CreateParticleInfo(10, P->position_, 2.0f, 30, 2.0f, 0.0f);
				//�_���[�WSE���Đ�
				Audio::GetInstance()->PlayWave("Damage.wav", 0.1f, false);
				//�v���C���[��HP���f�N�������g���ēG�̒e�̃f�X�t���O���グ��
				P->HP -= 1;
				bullet->DeathFlag = true;
			}
		}


		//�v���C���[�̒e�ƃ{�X�̓����蔻��
		if (Boss)
		{
			if (Boss->DeathFlag == false)
			{
				for (std::unique_ptr<Bullet>& bullet : bullets)
				{
					if (CheckCollision(bullet->GetPosition(), Boss->GetPosition(), 2.0f, 6.0f) == true)
					{
						//�p�[�e�B�N���𐶐�
						particle->CreateParticleInfo(10, Boss->position, 2.0f, 30, 1.0f, 0.0f);
						Audio::GetInstance()->PlayWave("EnemyDown.wav", 0.1f, false);
						Boss->HP -= 1;
						if (Boss->HP <= 0)
						{
							Audio::GetInstance()->PlayWave("BossDown.wav", 0.1f, false);
							//�{�X���j���̓p�[�e�B�N����h��ɉ��o
							particle->CreateParticleInfo(50, Boss->position, 2.0f, 50, 5.0f, 0.0f);
						}
						bullet->DeathFlag = true;
					}
				}
			}

			if (Boss->HP <= 0)
			{
				Boss->DeathFlag = true;
			}
		}

		//�{�X�̒e�ƃv���C���[�̓����蔻��
		if (Boss)
		{
			for (std::unique_ptr<BossBullet>& bullet : bossbullets)
			{
				//���@�ƓG�̒e�����蔻��m�F
				if (CheckCollision(P->GetPosition(), bullet->GetPosition(), 2.0f, 2.0f) == true)
				{
					if (P->HP == 1)
					{
						//�{�X���j���̓p�[�e�B�N����h��ɉ��o
						particle->CreateParticleInfo(50, P->position_, 2.0f, 50, 5.0f, 0.0f);
						Audio::GetInstance()->PlayWave("BossDown.wav", 0.1f, false);
					}
					P->HP -= 1;
					bullet->DeathFlag = true;

				}
			}
		}

		//�V���X�V
		CelestialSphere->Update();

		//���e�B�N���X�V
		//Reticle->Update(P->position_);

		//FBX�I�u�W�F�N�g�̍X�V
		//object1->Update();


		//�X�v���C�g�̍X�V

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

		//�J�����̍X�V
		camera->Update();

		//�p�[�e�B�N���X�V
		particle->Update();
		
		EnemyPart->Update();

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
	//SpriteCommon::GetInstance()->PreDraw();
	postEffect->PreDraw();

	////�X�v���C�g�`��



	/*SpriteCommon::PostDraw();*/
	
	//3D�I�u�W�F�N�g�̕`��O����
	Object3d::PreDraw();

	P->Draw();
	
	

	if (Boss)
	{
		if (Boss->DeathFlag == false)
		{
			Boss->Draw();
		}

		//�{�X�o��^�C�}�[�̃f�N�������g
		if (BossTimer > 0)
		{
			BossTimer--;
		}
	}

	//�G�̕`��
	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		if (enemy->DeathFlag == false)
		{
			enemy->Draw();
		}
	}

	//�v���C���[�̒e�`��
	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Draw();
	}

	//�G�̒e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
	{
		bullet->Draw();
	}

	//�{�X�̒e�`��
	for (std::unique_ptr<BossBullet>& bullet : bossbullets)
	{
		bullet->Draw();
	}

	//�V���̕`��
	CelestialSphere->Draw();

	//���e�B�N���`��
	//Reticle->Draw();

	//FBX�I�u�W�F�N�g�̕`��
	//object1->Draw(cmdList);

	if (P->Level >= 2)
	{
		CoreR->Draw();
	}

	if (P->Level >= 3)
	{
		CoreL->Draw();
	}

	//test->Draw();

	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();

	Particle::PreDraw();

	
	particle->Draw();

	EnemyPart->Draw();
	

	Particle::PostDraw();


	////�X�v���C�g�̋��ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
	
	//UI->Draw();

	if (Go->ComFlag_2 == true)
	{
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
	}

	Stage_1->Draw();
	Go->Draw();/*
	if (LoadFlag == false)
	{*/
		LoadBG->Draw();
	//}

	DrawSprite();

}

void GameScene::EnemyInit()
{
	//���˃^�C�}�[��������
	//EnemyBulletTimer = BulletInterval;

	for (std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->FireTime = enemy->IntervalTime;
	}
}

void GameScene::BossInit()
{
	//���˃^�C�}�[��������
	//EnemyBulletTimer = BulletInterval;

		Boss->FireTime = Boss->IntervalTime;
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

	//if (IsButtonPush(ButtonKind::Button_B))
	//{
	//	//�e�𐶐���������
	//	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	//	newBullet = Bullet::Create(model_Bullet, camera, StartPos, ReticlePos/*Reticle->position_*/);

	//	//�e��o�^
	//	bullets.push_back(std::move(newBullet));
	//}
}

void GameScene::EnemyAttack(XMFLOAT3 EnemyPos)
{
	
		//�e�𐶐���������
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet = EnemyBullet::Create(model_EnemyBullet, camera, EnemyPos, P->position_);

		//�e��o�^
		enemybullets.push_back(std::move(newBullet));

		//enemybullets
}

void GameScene::BossAttack(XMFLOAT3 BossPos)
{
	//�e�𐶐���������
	std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
	newBullet = BossBullet::Create(model_EnemyBullet, camera, BossPos, P);

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

		if (EnemyTimer <= 0)
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
		if (word.find("//") == 0)
		{
			//�R�����g���΂�
			continue;
		}

		//POP�R�}���h
		if (word.find("POP") == 0)
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
		else if (word.find("BOSS_POP") == 0)
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
			Boss = St1_Boss::Create(model_Boss, camera, EnemyPos);
		}

		//WAIT�R�}���h
		else if (word.find("WAIT") == 0)
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
}

void GameScene::DrawSprite()
{
	//�̗͉��o���̕`��

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

	//�{�X�̗͉��o��̕`��
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

		if (Boss->HP <= 0)
		{
			BossHP_0->Draw();
		}
	}
	
	//�o���lUI�̕`��

	if (Go->ComFlag_2 == true)
	{
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

		else if (P->EXP >= 5)
		{
			ExpBar_5->Draw();
		}

		//���x��
		if (P->Level == 1)
		{
			Level_1->Draw();
		}

		else if (P->Level == 2)
		{
			Level_2->Draw();
		}

		else if (P->Level >= 3)
		{
			Level_3->Draw();
		}

		//���[��UI
		Rule->Draw();
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
}

void GameScene::Start()
{
	//StartTimer += 1;
	P->MoveCanFlag = false;
	Stage_1->Vec.x = (Stage_1->PointPos.x - Stage_1->position_.x) / 30;
	Stage_1->position_.x += Stage_1->Vec.x;

	if (Stage_1->position_.x <= Stage_1->PointPos.x + 10)
	{
		Stage_1->PointPos.x = -400;
		Stage_1->PointPos.y = 285;
	}

	if (Stage_1->position_.x <= Stage_1->PointPos.x + 10)
	{
		Go->Vec.y = (Go->PointPos.y - Go->position_.y) / 30;
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
		if (P->position_.z < 0)
		{
			P->position_.z += 3;
		}


		P->MoveCanFlag = true;

	}

}



