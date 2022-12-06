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
	////�X�v���C�g���ʃe�N�X�`���ǂݍ���
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

	//	�X�v���C�g�̐���
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
	model_reticle = Model::LoadFromObj("Reticle");


	

	//�G�̏�����
	LoadEnemyPopData();
	EnemyInit();


	//3D�I�u�W�F�N�g����
	P = Player::Create(model_2, camera);

	//�V������
	CelestialSphere = Object3d::Create(model_sphere, camera);
	CelestialSphere->SetScale({100, 100, 100});

	//���e�B�N������
	Reticle = Reticle::Create(model_reticle, camera);
	Reticle->SetScale({ 2, 2, 2 });



		//�J���������_���Z�b�g
		camera->SetTarget({ 0, 20, 0 });


	/*object1->PlayAnimation();*/

	//���o�^�C�}�[������
	WaitTimer = 0;
	BossTimer = 128;
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
	//---�f�o�b�O�e�L�X�g�֌W---//

	//�}�E�X���W��2D���e�B�N���̃X�v���C�g�ɑ��
	UI->position_.x = Reticle->MousePosition.x;
	UI->position_.y = Reticle->MousePosition.y;

	

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

	//Reticle->Update();

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

	//���e�B�N���X�V
	Reticle->Update(P->position_);

	//FBX�I�u�W�F�N�g�̍X�V
	//object1->Update();
	
	/*UI->position_.x = MousePosition.x;
	UI->position_.y = MousePosition.y;*/

	//�X�v���C�g�̍X�V
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
	Reticle->Draw();

	//FBX�I�u�W�F�N�g�̕`��
	//object1->Draw(cmdList);


	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();

	////�X�v���C�g�̋��ʃR�}���h
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

void GameScene::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		//�e�𐶐���������
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet = Bullet::Create(model_Bullet, camera, P->position_, Reticle->position_);
		
		//�e��o�^
		bullets.push_back(std::move(newBullet));
	}
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

		if (Boss->HP == 0)
		{
			BossHP_0->Draw();
		}
	}
	
	//�o���lUI�̕`��
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

	//���x��
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



