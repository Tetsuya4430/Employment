#include "GameScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Fbx3d.h"
#include "FrameWork.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete(object1);
	delete(model1);
	delete(postEffect);
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

	//�I�u�W�F�N�g�}�l�[�W���[����
	objectManager_1 = std::make_unique<ObjectManager>();
	objectManager_2 = std::make_unique<ObjectManager>();
	objectManager_3 = std::make_unique<ObjectManager>();

	//�I�u�W�F�N�g�}�l�[�W���[����->3D�I�u�W�F�N�g����->�I�u�W�F�N�g�̎Q�ƃJ�E���g�𑝉�
	std::shared_ptr<Object3d> object_1 = Object3d::Create(model_1, camera);
	std::weak_ptr<Object3d> ObjectWp_1 = objectManager_1->AddObject(object_1);
	object3d_1 = ObjectWp_1.lock();

	std::shared_ptr<Object3d> object_2 = Object3d::Create(model_2, camera);
	std::weak_ptr<Object3d> ObjectWp_2 = objectManager_2->AddObject(object_2);
	BulletObj = ObjectWp_2.lock();

	std::shared_ptr<Object3d> object_3 = Object3d::Create(model_2, camera);
	std::weak_ptr<Object3d> ObjectWp_3 = objectManager_3->AddObject(object_3);
	Player = ObjectWp_3.lock();


	//3D�I�u�W�F�N�g�̈ʒu�Ɗg�嗦���w��
	
		object3d_1->SetScale({ 20.0f, 20.0f, 20.0f });

		BulletObj->SetPosition({ 0, 0, 0 });
		BulletObj->SetScale({ 0.5f, 0.5f, 0.5f });
	
		Player->SetPosition({ 0, 0, 0 });
		Player->SetScale({ 1.0f, 1.0f, 1.0f });
	

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

		//player->Initialize();

		

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

	//�v���C���[�̍��W���擾
	DirectX::XMFLOAT3 PlayerPos = Player->GetPosition();

	//�e�̃T�C�Y���擾
	DirectX::XMFLOAT3 BulletPos = BulletObj->GetPosition();

	if (Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && BulletFlag == 0)
	{
		BulletFlag = 1;
	}

	if (Input::GetInstance()->TriggerKey(DIK_A))
	{
		Audio::GetInstance()->PlayWave("Alarm01.wav");
	}

	//���W����
	if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))
	{
			if (Input::GetInstance()->PushKey(DIK_DOWN))
			{
				PlayerPos.y -= MoveVec;
				Player->SetPosition(PlayerPos);
			}

			if (Input::GetInstance()->PushKey(DIK_UP))
			{
				PlayerPos.y += MoveVec;
				Player->SetPosition(PlayerPos);
			}

			if (Input::GetInstance()->PushKey(DIK_LEFT))
			{
				PlayerPos.x -= MoveVec;
				Player->SetPosition(PlayerPos);
			}

			if (Input::GetInstance()->PushKey(DIK_RIGHT))
			{
				PlayerPos.x += MoveVec;
				Player->SetPosition(PlayerPos);
			}
	}

	if (BulletFlag == 0)
	{
		BulletObj->SetPosition(PlayerPos);
	}


	if (BulletFlag == 1)
	{
		BulletPos.z += 1.0f;

		BulletObj->SetPosition(BulletPos);
	}

	if (BulletPos.z >= 30)
	{
		BulletFlag = 0;
		BulletPos.z = 0;
		BulletObj->SetPosition(BulletPos);
	}


	//3D�I�u�W�F�N�g�̍X�V
	objectManager_1->Update();
	objectManager_2->Update();
	objectManager_3->Update();

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


	//�|�X�g�G�t�F�N�g�`��
	//postEffect->Draw();

	//3D�I�u�W�F�N�g�̕`��O����
	Object3d::PreDraw();

	////3D�I�u�W�F�N�g�̕`��
	//objectManager_1->Draw();

	//�e�̕`��
	if (BulletFlag == 1)
	{
		objectManager_2->Draw();
	}

	//�v���C���[�̕`��
	objectManager_3->Draw();

	//FBX�I�u�W�F�N�g�̕`��
	object1->Draw(cmdList);

	//3D�I�u�W�F�N�g�̕`��㏈��
	Object3d::PostDraw();


	//////�X�v���C�g�̋��ʃR�}���h
	//SpriteCommon::GetInstance()->PreDraw();

	//////�X�v���C�g�`��
	//sprite->Draw();
}


