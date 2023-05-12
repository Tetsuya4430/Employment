#include "FrameWork.h"
#include "imgui.h"

void FrameWork::Run()
{
	//�Q�[���̏�����
	Initialize();

	while (true)	//�Q�[�����[�v
	{
		//���t���[���X�V
		Update();

		//�I�����N�G�X�g�������甲����
		if (IsEndRequest())
		{
			break;
		}
		//�`��
		Draw();
	}
	//�Q�[���̏I��
	Finalize();
}

void FrameWork::Initialize()
{
	//WindowsAPI�̏�����
	winApp = WinApp::GetInstance();
	winApp->Initialize();

	//DirectX�̏�����
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = SpriteCommon::GetInstance();
 	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	//�f�o�b�O�e�L�X�g
	debugText = DebugText::GetInstance();

	//�f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	const int debugTextTexNumber = 0;
	//�f�o�b�O�e�L�X�g�p�̃e�N�X�`���ǂݍ���
	spriteCommon->SpriteCommonLoadTexture(debugTextTexNumber, L"Resources/Font/debugfont.png");
	//�f�o�b�O�e�L�X�g������
	debugText->Initialize(debugTextTexNumber);

	//���͂̏�����
	input = Input::GetInstance();
	input->Initialize(winApp);

	//�I�[�f�B�I�̏�����
	audio = Audio::GetInstance();
	audio->Initialize();

	//ImGui�̏�����
	ImGui = ImGuiManager::GetInstance();
	ImGui->Initialize(winApp, dxCommon);

	//3D�I�u�W�F�N�g�̐ÓI������
	Object3d::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//�v���C���[�̐ÓI������
	Player::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	
	//���e�B�N���̐ÓI������
	//Reticle::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//�G�̐ÓI������
	Enemy::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//�e�I�u�W�F�N�g�̐ÓI������
	Bullet::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	//�G�̒e�I�u�W�F�N�g�̐ÓI������
	EnemyBullet::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//�X�e�[�W1�̃{�X�I�u�W�F�N�g�̐ÓI������
	Boss::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//�{�X�̃p�[�c�̐ÓI������
	BossParts::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//�{�X�e
	BossBullet::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	//FBX���[�_�[�̏���������
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	//�p�[�e�B�N��������
	Particle::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	DefenceObject::StaticInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
}

void FrameWork::Finalize()
{
	//�f�o�b�O�e�L�X�g���
	DebugText::GetInstance()->Finalize();
	
	//�V�[���}�l�[�W���[�̉��
	SceneManager::GetInstance()->Finalize();

	//FBX���[�_�[�̉��
	FbxLoader::GetInstance()->Finalize();

	//�I�[�f�B�I���
	audio->Finalize();


	//ImGui�̉��
	ImGui->Finalize();

	//DirectX���
	//delete dxCommon;

	//Windows��API�̏I������
	winApp->Finalie();
}

void FrameWork::Update()
{
	//Windows�̃��b�Z�[�W����
	if (winApp->ProcessMessage())
	{
		//�Q�[�����[�v�𔲂���
		endRequest_ = true;
		return;
	}

	//���͂̍X�V
	input->Update();

	//�V�[���̍X�V
	SceneManager::GetInstance()->Update();
}

void FrameWork::Draw()
{
	//�`��O����
	dxCommon->PreDraw();
	//ImGui�̍X�V
	ImGui->GetInstance()->ImGuiManager::Begin();
	SceneManager::GetInstance()->Draw();

	//�f�o�b�O�e�L�X�g�`��
	debugText->DrawAll();
	//ImGui::Text("Hello, world %d", 123);

	//4,�`��R�}���h�����܂�
	//ImGui�̕`��
	ImGui->GetInstance()->ImGuiManager::End();
	ImGui->GetInstance()->ImGuiManager::Draw();
	dxCommon->PostDraw();
}
