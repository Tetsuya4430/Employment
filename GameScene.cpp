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
	////�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteCommon::GetInstance()->SpriteCommonLoadTexture(1, L"Resources/Image/GamePlay.png");
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

	

	//3D�I�u�W�F�N�g����
	player = Object3d::Create(model_2, camera);

	P = Player::Create(model_2, camera);

	E = Enemy::Create(model_Enemy, camera);

	
	

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

	


	//3D�I�u�W�F�N�g�̍X�V


	player->Update();

	P->Update();

	E->Update();

	Attack();

	//�e�X�V
	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Update(P->position_);
	}

	

	/*if (bullets)
	{
		bullet->Update(P->position_);
	}*/

	//B->Update();

	//FBX�I�u�W�F�N�g�̍X�V
	object1->Update();
	
	//�X�v���C�g�̍X�V
	sprite->Update();


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



	P->Draw();

	E->Draw();



	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Draw();
	}

	

	//FBX�I�u�W�F�N�g�̕`��
	object1->Draw(cmdList);

	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();

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


