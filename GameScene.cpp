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
	delete(particle);
}

void GameScene::Initialize()
{ 
	////�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/BackGround.png");
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(100, L"Resources/Image/Sana.png");

	//	�X�v���C�g�̐���
	sprite = Sprite::Create(1, { 0, 0 }, false, false);

	//�|�X�g�G�t�F�N�g�̐���
	postEffect = PostEffect::Create(100, { 0, 0 }, false, false);

	//OBJ���烂�f���f�[�^��ǂݍ���
	model_1 = Model::LoadFromObj("triangle_mat");
	model_2 = Model::LoadFromObj("Box");
	model_Bullet = Model::LoadFromObj("Bullet");
	model_Enemy = Model::LoadFromObj("Enemy");

	//�G�̏�����
	EnemyInit();


	//3D�I�u�W�F�N�g����
	player = Object3d::Create(model_2, camera);

	P = Player::Create(model_2, camera);

	//E = Enemy::Create(model_Enemy, camera);

	enemy = std::make_unique<Enemy>();
	
	enemy = Enemy::Create(model_Enemy, camera);

	part = Part::Create(model_1, camera);

	//�G�̒e�Ɏ��L�����ɃA�h���X��n��
	//enemybullets

		//�f�o�C�X���Z�b�g
		Fbx3d::SetDevice(dxCommon->GetDev());
		//�J�����Z�b�g
		Fbx3d::SetCamera(camera);
		//�O���t�B�b�N�X�p�C�v���C���𐶐�
		Fbx3d::CreateGraphicsPipeline();

		//���f�����w�肵��FBX�t�@�C����ǂݍ���
		model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

		//3d�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
		object1 = new Fbx3d;
		object1->Initialize();
		object1->SetModel(model1);

		object1->SetPosition({ 100.0f, 0.0f, 0.0f });

		//�p�[�e�B�N���̐���
		particle = Particle::Create(camera);
		particle->Update();

		

		//�J���������_���Z�b�g
		camera->SetTarget({ 0, 20, 0 });

	//�����ǂݍ��݂ƍĐ�
	Audio::GetInstance()->LoadWave("Alarm01.wav");


	object1->PlayAnimation();
}

void GameScene::Finalize()
{
	//�X�v���C�g���
	delete sprite;

	//���f���̉��
	delete model_1;
	delete model_2;
}

void GameScene::Update()
{
	//---�f�o�b�O�e�L�X�g�֌W---//
	//X���W�AY���W���w�肵�ĕ\��
	DebugText::GetInstance()->Print("Debug Text", 0, 0);

	//X���W�AY���W�A�k�ڂ��w�肵�ĕ\��
	DebugText::GetInstance()->Print("Debug Text = 0", 0, 50, 2.0f);

	

	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	

	//// �J�����ړ�
	//if (input->GetInstance()->PushKey(DIK_I) || input->GetInstance()->PushKey(DIK_J) || input->GetInstance()->PushKey(DIK_K) || input->GetInstance()->PushKey(DIK_L))
	//{
	//	if (input->GetInstance()->PushKey(DIK_I)) { Camera::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
	//	else if (input->GetInstance()->PushKey(DIK_K)) { Camera::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
	//	if (input->GetInstance()->PushKey(DIK_L)) { Camera::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
	//	else if (input->GetInstance()->PushKey(DIK_J)) { Camera::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }
	//}

	//camera->Update();

	//3D�I�u�W�F�N�g�̍X�V


	player->Update();

	P->Update();

	//E->Update();

	if (enemy)
	{
		enemy->Update();
	}

	Attack();

	//EnemyAttack();

	//�G�X�V
	if (enemy->DeathFlag == false)
	{
		EnemyUpdate();
	}

	//�e�X�V
	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Update(P->position_);
	}

	//�G�̒e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
	{
		bullet->Update(enemy->position, enemy->position);
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

	for (std::unique_ptr<Bullet>& bullet : bullets)
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

	
	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
	{
		//���@�ƓG�̒e�����蔻��m�F
		if (CheckCollision(P->GetPosition(), bullet->GetPosition(), 2.0f, 2.0f) == true)
		{
			bullet->DeathFlag = true;
		}
	}

	//FBX�I�u�W�F�N�g�̍X�V
	object1->Update();
	
	//�X�v���C�g�̍X�V
	sprite->Update();

	//�p�[�e�B�N���̍X�V
	particle->Update();

	part->Update();


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

	//3D�I�u�W�F�N�g�̕`��O����
	Object3d::PreDraw();
	//Particle::PreDraw();

	P->Draw();

	//E->Draw();

	if (enemy->DeathFlag == false)
	{
		enemy->Draw();
	}


	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Draw();
	}

	for (std::unique_ptr<EnemyBullet>& bullet : enemybullets)
	{
		bullet->Draw();
	}


	part->Draw();

	//FBX�I�u�W�F�N�g�̕`��
	object1->Draw(cmdList);

	//particle->Draw();

	//Particle::PostDraw();
	

	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();

}

void GameScene::EnemyInit()
{
	//���˃^�C�}�[��������
	EnemyBulletTimer = BulletInterval;
}

void GameScene::EnemyUpdate()
{
	EnemyBulletTimer--;

	if (EnemyBulletTimer <= 0)
	{
		EnemyAttack(enemy->GetPosition());

		//���˃^�C�}�[��������
		EnemyBulletTimer = BulletInterval;
	}
}

void GameScene::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_R))
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
	/*if (Input::GetInstance()->TriggerKey(DIK_T))
	{*/



		//�e�𐶐���������
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet = EnemyBullet::Create(model_Bullet, camera, enemy->position, P);

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
	//}
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


