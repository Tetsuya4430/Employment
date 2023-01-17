#include "Player.h"
#include "Input.h"
#include "Bullet.h"
#include "Controller.h"
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#pragma comment(lib, "d3dcompiler.lib")

#include "DebugText.h"
#include <iomanip>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* Player::device = nullptr;
ID3D12GraphicsCommandList* Player::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Player::rootsignature;
ComPtr<ID3D12PipelineState> Player::pipelinestate;

Player* Player::Create(Model* model, Camera* camera)
{
    //3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
    Player* instance = new Player();
    if (instance == nullptr)
    {
        return nullptr;
    }

    //������
    if (!instance->Initialize())
    {
        delete instance;
        assert(0);
    }

    //���f���̃Z�b�g
    if (model)
    {
        instance->SetModel(model);
    }

    //�J�����̃Z�b�g
    if (camera)
    {
        instance->SetCamera(camera);
    }

    return instance;
}

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

bool Player::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	//�R���g���[���[������
	InitInput();

	//���e�B�N��������
	ReticlePos.x = position_.x;
	ReticlePos.y = position_.y;
	ReticlePos.z = position_.z + 30.0f;

	//�v���C���[�e��X�e�[�^�X������
	position_.z = -50;
	position_.y = -10;
	Level = 1;
	EXP = 0;

	audio->Initialize();


	Audio::GetInstance()->LoadWave("Avoid.wav");
	Audio::GetInstance()->LoadWave("LevelUp.wav");

	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0_));

    return true;
}

bool Player::Finalize()
{
	ReleaseInput();

	return true;
}

void Player::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(Player::cmdList);

	//���f���̕R�Â����Ȃ��ꍇ�͕`�悵�Ȃ�
	if (model_ == nullptr)
	{
		return;
	}


	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());

	model_->Draw(cmdList, 1);
}

void Player::Update()
{
	/*if (position_.z < 0)
	{
		position_.z += 3;
	}*/

	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ���[���h�s��̍���
	matWorld_ = XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld_ *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld_ *= matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	if (parent_ != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld_ *= parent_->matWorld_;
	}

	const XMMATRIX& matViewProjection = camera_->GetmatViewProjection();

	// �萔�o�b�t�@�փf�[�^�]��B0
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0_->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	//constMap->mat = matWorld_ * matView * matProjection;	// �s��̍���
	constMap->mat = matWorld_ * matViewProjection;	// �s��̍���
	constBuffB0_->Unmap(0, nullptr);

	//�X�V����

	Mag = 0.0f;

	if (Level == 1)
	{
		Speed = 0.5f;
	}

	else if (Level == 2)
	{
		Speed = 0.75f;
	}

	else if (Level == 3)
	{
		Speed = 1.0f;
	}

	

	//�R���g���[���[�̉������X�V
	UpdateInput();

	//�v���C���[�̈ړ�
	//�L�[�{�[�h
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S))
	{
		
			K = 0.5f;
		
			if (MoveCanFlag == true)
			{


				if (Input::GetInstance()->PushKey(DIK_A) && Input::GetInstance()->PushKey(DIK_W))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_A) && Input::GetInstance()->PushKey(DIK_S))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_D) && Input::GetInstance()->PushKey(DIK_W))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_D) && Input::GetInstance()->PushKey(DIK_S))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else
				{
					Mag = 1.0f;

					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}


				MoveFlag = 1;
				if (Input::GetInstance()->PushKey(DIK_D))
				{
					moveR = true;
					if (position_.x <= 45)
					{
						position_.x += Speed * Mag;
						Old.x = Speed * Mag;
					}
				}


				if (Input::GetInstance()->PushKey(DIK_A))
				{
					moveL = true;
					if (position_.x >= -45)
					{
						position_.x -= Speed * Mag;
						Old.x = -Speed * Mag;
					}
				}


				if (Input::GetInstance()->PushKey(DIK_W))
				{
					moveU = true;
					if (position_.y <= 20)
					{
						position_.y += Speed * Mag;
						Old.y = Speed * Mag;
					}
				}

				if (Input::GetInstance()->PushKey(DIK_S))
				{
					moveD = true;
					if (position_.y >= -25)
					{
						position_.y -= Speed * Mag;
						Old.y = -Speed * Mag;
					}
				}

				else
				{
					MoveFlag = 0;
				}
			}
			
			//�v���C���[���ړ����Ă��Ȃ��Ƃ��̓v���C���[�̉�]�p�x�����X�ɖ߂�
			if (MoveFlag == 0)
			{
				if (Avoidanceflag_X == false && Avoidanceflag_Y == false)
				{
					/*if (rotation_.z > 0)
					{
						rotation_.z -= 0.5f;
					}

					if (rotation_.z < 0)
					{
						rotation_.z += 0.5f;
					}*/
				}
			}


			if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			{
				MoveFlag = 1;
				if (IsButtonPush(ButtonKind::RightButton))
				{
					position_.x += 0.5f;
					if (RotlimR < rotation_.z)
					{
						rotation_.z -= 0.3f;
					}
				}

				if (IsButtonPush(ButtonKind::LeftButton))
				{
					MoveFlag = 1;
					position_.x -= 0.5f;
					if (RotlimL > rotation_.z)
					{
						rotation_.z += 0.3f;
					}
				}

				if (IsButtonPush(ButtonKind::UpButton))
				{
					position_.y += 0.5f;
				}

				if (IsButtonPush(ButtonKind::DownButton))
				{
					position_.y -= 0.5f;
				}
			}

			else
			{
				MoveFlag = 0;
			}


		//}
	}


	//�L�[�{�[�h
	if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
	{

		K = 0.5f;

		if (MoveCanFlag == true)
		{


			if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::UpButton))
			{
				Mag = 0.71f;
				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}

			else if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::DownButton))
			{
				Mag = 0.71f;
				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}

			else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::UpButton))
			{
				Mag = 0.71f;
				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}

			else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::DownButton))
			{
				Mag = 0.71f;
				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}

			else
			{
				Mag = 1.0f;

				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}


			MoveFlag = 1;
			if (IsButtonPush(ButtonKind::RightButton))
			{
				moveR = true;
				if (position_.x <= 45)
				{
					position_.x += Speed * Mag;
					Old.x = Speed * Mag;
				}
			}


			if (IsButtonPush(ButtonKind::LeftButton))
			{
				moveL = true;
				if (position_.x >= -45)
				{
					position_.x -= Speed * Mag;
					Old.x = -Speed * Mag;
				}
			}


			if (IsButtonPush(ButtonKind::UpButton))
			{
				moveU = true;
				if (position_.y <= 20)
				{
					position_.y += Speed * Mag;
					Old.y = Speed * Mag;
				}
			}

			if (IsButtonPush(ButtonKind::DownButton))
			{
				moveD = true;
				if (position_.y >= -25)
				{
					position_.y -= Speed * Mag;
					Old.y = -Speed * Mag;
				}
			}

			else
			{
				MoveFlag = 0;
			}
		}


		//�v���C���[���ړ����Ă��Ȃ��Ƃ��̓v���C���[�̉�]�p�x�����X�ɖ߂�
		if (MoveFlag == 0)
		{
			if (Avoidanceflag_X == false && Avoidanceflag_Y == false)
			{
				/*if (rotation_.z > 0)
				{
					rotation_.z -= 0.5f;
				}

				if (rotation_.z < 0)
				{
					rotation_.z += 0.5f;
				}*/
			}
		}


		if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
		{
			MoveFlag = 1;
			if (IsButtonPush(ButtonKind::RightButton))
			{
				position_.x += 0.5f;
				if (RotlimR < rotation_.z)
				{
					rotation_.z -= 0.3f;
				}
			}

			if (IsButtonPush(ButtonKind::LeftButton))
			{
				MoveFlag = 1;
				position_.x -= 0.5f;
				if (RotlimL > rotation_.z)
				{
					rotation_.z += 0.3f;
				}
			}

			if (IsButtonPush(ButtonKind::UpButton))
			{
				position_.y += 0.5f;
			}

			if (IsButtonPush(ButtonKind::DownButton))
			{
				position_.y -= 0.5f;
			}
		}

		else
		{
			MoveFlag = 0;
		}


		//�R���g���[���[�̉������X�V
		UpdateInput();
		//}
	}

	//�����ړ����Ă��Ȃ��̂ł����
	if (Mag == 0.0f)
	{
		//�ړ��ʕ�������
		if (K > 0)
		{
			if (moveR == true)
			{
				K -= 0.01f;
				position_.x += K;
			}

			if (moveL == true)
			{
				K -= 0.01f;
				position_.x -= K;
			}

			if (moveU == true)
			{
				K -= 0.01f;
				position_.y += K;
			}

			if (moveD == true)
			{
				K -= 0.01f;
				position_.y -= K;
			}

			//�΂߈ړ�
			else if (moveR == true && moveU == true)
			{
				K -= 0.01f;
				position_.x += K;
				position_.y += K;
			}

			else if (moveR == true && moveD == true)
			{
				K -= 0.01f;
				position_.x += K;
				position_.y -= K;
			}

			else if (moveL == true && moveD == true)
			{
				K -= 0.01f;
				position_.x -= K;
				position_.y -= K;
			}

			else if (moveL == true && moveU == true)
			{
				K -= 0.01f;
				position_.x -= K;
				position_.y += K;
			}
		}

	}

//�f�o�b�O�p
	//�I�u�W�F�N�g�̍��W
	//std::ostringstream Objstr;
	//Objstr << "ObjPos("
	//	<< std::fixed << std::setprecision(5)
	//	<< position_.x << ","
	//	<< position_.y << ","
	//	<< position_.z << ")";

	//DebugText::GetInstance()->Print(Objstr.str(), 0, 300, 2.0f);

	////mag
	//std::ostringstream Magstr;
	//Magstr << "Mag("
	//	<< std::fixed << std::setprecision(5)
	//	<< Mag << ")";

	//DebugText::GetInstance()->Print(Magstr.str(), 0, 0, 2.0f);

	////Old
	//std::ostringstream Oldstr;
	//Oldstr << "OldPos("
	//	<< std::fixed << std::setprecision(5)
	//	<< Old.x << ","
	//	<< Old.y << ")";

	//DebugText::GetInstance()->Print(Oldstr.str(), 0, 50, 2.0f);


	////�����ړ����I�����Ă���Ȃ�
	//if (Mag == 0.0)
	//{
	//	//�܂��ړ��ʂ�0�łȂ��̂Ȃ��
	//	if (Old.x != 0.0f)
	//	{
	//		if (Old.x < 0.0f)
	//		{
	//			Old.x += 0.1f;
	//		}

	//		if (Old.x > 0.0f)
	//		{
	//			Old.x -= 0.1f;
	//		}
	//	}

	//	//�܂��ړ��ʂ�0�łȂ��̂Ȃ��
	//	if (Old.y != 0.0f)
	//	{
	//		if (Old.y < 0.0f)
	//		{
	//			Old.y += 0.1f;
	//		}

	//		if (Old.y > 0.0f)
	//		{
	//			Old.y -= 0.1f;
	//		}
	//	}

	//	//�ړ��̕�������
	//	position_.x += Old.x;
	//	position_.y += Old.y;
	//}

	//������[�V����
	//�L�[�{�[�h
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S))
	{

		if (AvoidanceTimer_X == 0)
		{
			//�E���
			if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_D) && Avoidanceflag_X == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.x + AvoidDistance_X;
				RolePos = rotation_.z - RoleDistance;
				RotFlag_R = true;

				Avoidanceflag_X = true;
			}

			//�����
			if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_A) && Avoidanceflag_X == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.x - AvoidDistance_X;
				RolePos = rotation_.z + RoleDistance;
				RotFlag_L = true;

				Avoidanceflag_X = true;
			}
		}

		if (AvoidanceTimer_Y == 0)
		{
			//����
			if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_W) && Avoidanceflag_Y == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.y + AvoidDistance_Y;

				Avoidanceflag_Y = true;
			}

			//�����
			if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_S) && Avoidanceflag_Y == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.y - AvoidDistance_Y;

				Avoidanceflag_Y = true;
			}
		}
	}

	//�΂߉���͎󂯕t���Ȃ�
	if (Input::GetInstance()->PushKey(DIK_W) && Input::GetInstance()->PushKey(DIK_D))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (Input::GetInstance()->PushKey(DIK_W) && Input::GetInstance()->PushKey(DIK_A))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (Input::GetInstance()->PushKey(DIK_S) && Input::GetInstance()->PushKey(DIK_D))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (Input::GetInstance()->PushKey(DIK_S) && Input::GetInstance()->PushKey(DIK_A))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	//�R���g���[���[
	if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
	{

		if (AvoidanceTimer_X == 0)
		{
			//�E���
			if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::RightButton) && Avoidanceflag_X == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.x + AvoidDistance_X;
				RolePos = rotation_.z - RoleDistance;
				RotFlag_R = true;

				Avoidanceflag_X = true;
			}

			//�����
			if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::LeftButton) && Avoidanceflag_X == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.x - AvoidDistance_X;
				RolePos = rotation_.z + RoleDistance;
				RotFlag_L = true;

				Avoidanceflag_X = true;
			}
		}

		if (AvoidanceTimer_Y == 0)
		{
			//����
			if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::UpButton) && Avoidanceflag_Y == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.y + AvoidDistance_Y;

				Avoidanceflag_Y = true;
			}

			//�����
			if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::DownButton) && Avoidanceflag_Y == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.y - AvoidDistance_Y;

				Avoidanceflag_Y = true;
			}
		}
	}

	//�΂߉���͎󂯕t���Ȃ�
	if (IsButtonPush(ButtonKind::UpButton) && IsButtonPush(ButtonKind::RightButton))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (IsButtonPush(ButtonKind::UpButton) && IsButtonPush(ButtonKind::LeftButton))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (IsButtonPush(ButtonKind::DownButton) && IsButtonPush(ButtonKind::RightButton))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (IsButtonPush(ButtonKind::DownButton) && IsButtonPush(ButtonKind::LeftButton))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	//���ۂ̉������
	if (Avoidanceflag_X == true)
	{
		AvoidanceTimer_X += 1;

		
		dx = (PointPos - position_.x) / AvoidCount;
		position_.x += dx;

		if (RotFlag_R == true)
		{
			rx = (RolePos - rotation_.z) / AvoidCount;
			rotation_.z += rx;
		}

		else if (RotFlag_L == true)
		{
			rx = (RolePos - rotation_.z) / AvoidCount;
			rotation_.z += rx;
		}

		if (AvoidanceTimer_X >= 20)
		{
			AvoidanceTimer_X = 0;
			rotation_.z = 0;
			RotFlag_R = false;
			RotFlag_L = false;
			Avoidanceflag_X = false;
		}

	}

	if (Avoidanceflag_Y == true)
	{
		AvoidanceTimer_Y += 1;

		
		dx = (PointPos - position_.y) / AvoidCount;
		position_.y += dx;

		if (AvoidanceTimer_Y >= 20)
		{
			AvoidanceTimer_Y = 0;
			Avoidanceflag_Y = false;
		}
	}


	//�v���C���[����ʔ��O�ɐi�o���悤�Ƃ���Ƃ��͉����߂�
	//�E��
	if (position_.x >= 45)
	{
		position_.x = 45;
	}

	//����
	if (position_.x <= -45)
	{
		position_.x = -45;
	}

	//�㑤
	if (position_.y >= 20)
	{
		position_.y = 20;
	}

	//����
	if (position_.y <= -20)
	{
		position_.y = -20;
	}

	//���x���̊Ǘ�
	//�o���l��5���܂�����
	if (EXP == 5 && Level < 3)
	{
		Audio::GetInstance()->PlayWave("LevelUp.wav", 0.3f, false);
		//�o���l�����Z�b�g���āA���x����1�㏸
		EXP = 0;
		Level += 1;
	}
}


XMFLOAT3 Player::GetWorldPosition()
{
	//���[���h���W������ϐ�
	XMFLOAT3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = position_.x;
	worldPos.y = position_.y;
	worldPos.z = position_.z;

	return worldPos;
}

bool Player::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{// nullptr�`�F�b�N
    assert(device);

	Player::device = device;
	Player::cmdList = cmdList;

    Model::SetDevice(device);

    // �J����������
    Camera::InitializeCamera(window_width, window_height);

    // �p�C�v���C��������
    InitializeGraphicsPipeline();

    // �e�N�X�`���ǂݍ���
    //LoadTexture();


    return true;
}



bool Player::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJVS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJPS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return result;
	}

	return true;
}
