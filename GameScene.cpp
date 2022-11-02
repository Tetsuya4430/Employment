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
	////�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/BackGround.png");
	//SpriteCommon::GetInstance()->SpriteCommonLoadTexture(100, L"Resources/Image/Sana.png");

	//	�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0, 0 }, false, false);

	//�|�X�g�G�t�F�N�g�̐���
	//postEffect = PostEffect::Create(100, { 0, 0 }, false, false);

	//OBJ���烂�f���f�[�^��ǂݍ���
	//model_1 = Model::LoadFromObj("triangle_mat");
	model_2 = Model::LoadFromObj("Player");
	model_Bullet = Model::LoadFromObj("Bullet");
	model_Enemy = Model::LoadFromObj("Enemy");
	model_Boss = Model::LoadFromObj("Boss");
	model_sphere = Model::LoadFromObj("CelestialSphere");


	

	//�G�̏�����
	LoadEnemyPopData();
	EnemyInit();


	//3D�I�u�W�F�N�g����
	P = Player::Create(model_2, camera);

	CelestialSphere = Object3d::Create(model_sphere, camera);
	CelestialSphere->SetScale({100, 100, 100});

	//part = Part::Create(model_1, camera);

	//�G�̒e�Ɏ��L�����ɃA�h���X��n��
	//enemybullets

		////�f�o�C�X���Z�b�g
		//Fbx3d::SetDevice(dxCommon->GetDev());
		////�J�����Z�b�g
		//Fbx3d::SetCamera(camera);
		////�O���t�B�b�N�X�p�C�v���C���𐶐�
		//Fbx3d::CreateGraphicsPipeline();

		//���f�����w�肵��FBX�t�@�C����ǂݍ���
	//	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

		//3d�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
	/*	object1 = new Fbx3d;
		object1->Initialize();
		object1->SetModel(model1);

		object1->SetPosition({ 100.0f, 0.0f, 0.0f });*/

		////�p�[�e�B�N���̐���
		//particle = Particle::Create(camera);
		//particle->Update();

		

		//�J���������_���Z�b�g
		camera->SetTarget({ 0, 20, 0 });

	//�����ǂݍ��݂ƍĐ�
	Audio::GetInstance()->LoadWave("Alarm01.wav");


	/*object1->PlayAnimation();*/

	//���o�^�C�}�[������
	WaitTimer = 0;
}

void GameScene::Finalize()
{
	//�X�v���C�g���
	delete sprite;

	//���f���̉��
	//delete model_1;
	delete model_2;
}

void GameScene::Update()
{
	//---�f�o�b�O�e�L�X�g�֌W---//
	////X���W�AY���W���w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Debug Text", 0, 0);

	////X���W�AY���W�A�k�ڂ��w�肵�ĕ\��
	//DebugText::GetInstance()->Print("Debug Text = 0", 0, 50, 2.0f);

	
	//�V�[���J��
	//�G��HP��0�ɂȂ�����Q�[���N���A
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
		//�v���C���[��HP��0�ɂȂ�����Q�[���I�[�o�[
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
	

	//3D�I�u�W�F�N�g�̍X�V


	//player->Update();

	//�v���C���[�̍X�V
	P->Update();

	//�v���C���[�̍U���֐�
	Attack();

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
	bullets.remove_if([](std::unique_ptr<Bullet>&bullet)
	{
		return bullet->DeathGetter();
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

	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		for (std::unique_ptr<Enemy>& enemy : enemys)
		{
			//�����蔻��m�F
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
		//���@�ƓG�̒e�����蔻��m�F
		if (CheckCollision(P->GetPosition(), bullet->GetPosition(), 2.0f, 2.0f) == true)
		{
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

	//�{�X�̒e�ƃv���C���[�̓����蔻��
	if (Boss)
	{
		for (std::unique_ptr<BossBullet>& bullet : bossbullets)
		{
			//���@�ƓG�̒e�����蔻��m�F
			if (CheckCollision(P->GetPosition(), bullet->GetPosition(), 2.0f, 2.0f) == true)
			{
				P->HP -= 1;
				bullet->DeathFlag = true;
			}
		}
	}

	//�V���X�V
	CelestialSphere->Update();

	//FBX�I�u�W�F�N�g�̍X�V
	//object1->Update();
	
	//�X�v���C�g�̍X�V
	sprite->Update();

	//�p�[�e�B�N���̍X�V
	//particle->Update();

	//part->Update();


	//�J�����̍X�V
	camera->Update();


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

	////�X�v���C�g�̋��ʃR�}���h
	SpriteCommon::GetInstance()->PreDraw();
	postEffect->PreDraw();

	////�X�v���C�g�`��
	sprite->Draw();

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

	//part->Draw();

	//FBX�I�u�W�F�N�g�̕`��
	//object1->Draw(cmdList);


	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();

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
	

	//for (std::unique_ptr<Enemy>& enemy : enemys)
	//{
	//	if (EnemyBulletTimer <= 0 /*&& enemy->DeathFlag == 0*/)
	//	{
	//		EnemyAttack(/*enemy->position*/ enemyPos);

	//		//���˃^�C�}�[��������
	//		EnemyBulletTimer = BulletInterval;
	//	}
	//}	
}

void GameScene::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		//�e�𐶐���������
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet = Bullet::Create(model_Bullet, camera, P->position_);
		
		//�e��o�^
		bullets.push_back(std::move(newBullet));
	}
}

void GameScene::EnemyAttack(XMFLOAT3 EnemyPos)
{
	
		//�e�𐶐���������
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

		//�e��o�^
		enemybullets.push_back(std::move(newBullet));

		//enemybullets
}

void GameScene::BossAttack(XMFLOAT3 BossPos)
{
	//�e�𐶐���������
	std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
	newBullet = BossBullet::Create(model_Bullet, camera, BossPos, P);

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



