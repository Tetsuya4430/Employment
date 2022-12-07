#include "Reticle.h"
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
ID3D12Device* Reticle::device = nullptr;
ID3D12GraphicsCommandList* Reticle::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Reticle::rootsignature;
ComPtr<ID3D12PipelineState> Reticle::pipelinestate;

Reticle* Reticle::Create(Model* model, Camera* camera)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Reticle* instance = new Reticle();
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

Reticle* Reticle::GetInstance()
{
	static Reticle instance;

	return &instance;
}

bool Reticle::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	//�R���g���[���[������
	InitInput();

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

bool Reticle::Finalize()
{
	ReleaseInput();

	return true;
}

void Reticle::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(Reticle::cmdList);

	//���f���̕R�Â����Ȃ��ꍇ�͕`�悵�Ȃ�
	if (model_ == nullptr)
	{
		return;
	}


	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());

	model_->Draw(cmdList, 1);
}

void Reticle::Update(XMFLOAT3 PlayerPos)
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	//�}�E�X���W�擾
	GetCursorPos(&MousePosition);
	ShowCursor(false);	//�J�[�\����\�����Ȃ�

	//�N���C�A���g�G���A���W�ɕϊ�
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &MousePosition);

	//�r���[�|�[�g�s��
	XMMATRIX matViewport = XMMatrixIdentity();	//�P�ʍs��

	matViewport.r[0].m128_f32[0] = 640;
	matViewport.r[1].m128_f32[1] = -360;
	matViewport.r[3].m128_f32[0] = 640;
	matViewport.r[3].m128_f32[1] = 360;

	//�r���[�v���W�F�N�V�����r���[�|�[�g�����s��
	XMMATRIX matVPV = camera_->matView * camera_->matProjection * matViewport;

	//�����s��̋t�s������v�Z����
	XMMATRIX matInverseVPV = XMMatrixInverse(nullptr, matVPV);

	//�X�N���[�����W
	XMVECTOR PosNear = { MousePosition.x, MousePosition.y, 0 };
	XMVECTOR PosFar = { MousePosition.x, MousePosition.y, 1 };

	//�X�N���[�����W�n���烏�[���h���W�n��
	PosNear = XMVector3Transform(PosNear, matInverseVPV);
	PosFar = XMVector3Transform(PosFar, matInverseVPV);

	//�}�E�X���C�̕���
	XMVECTOR mouseDirection = PosNear - PosFar;
	mouseDirection = XMVector3Normalize(mouseDirection);

	//�J��������Ə��I�u�W�F�N�g�̋���
	const float kDistanceTestObject = 30.0f;

	//XMVECTOR VecPos;

	//VecPos = XMLoadFloat3(&position_);

	//XMLoadFloat3(&position_) = PosNear - mouseDirection + XMLoadFloat(&kDistanceTestObject);

	XMStoreFloat3(&position_, PosNear - mouseDirection + XMLoadFloat(&kDistanceTestObject));

	//position_.z = 20.0f;

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

	

	//�f�o�b�O�e�L�X�g
	//�}�E�X���W
	std::ostringstream Mousestr;
	Mousestr << "MousePosition("
		<< std::fixed << std::setprecision(2)
		<< MousePosition.x << ","
		<< MousePosition.y << ")";

	DebugText::GetInstance()->Print(Mousestr.str(), 0, 50, 2.0f);

	//�j�A
	std::ostringstream Nearstr;
	Nearstr << "PosNear("
		<< std::fixed << std::setprecision(5)
		<< PosNear.m128_f32[0] << ","
		<< PosNear.m128_f32[1] << ","
		<< PosNear.m128_f32[2] << ")";

	DebugText::GetInstance()->Print(Nearstr.str(), 0, 200, 2.0f);

	//�t�@�[
	std::ostringstream Farstr;
	Farstr << "PosFar("
		<< std::fixed << std::setprecision(5)
		<< PosFar.m128_f32[0] << ","
		<< PosFar.m128_f32[1] << ","
		<< PosFar.m128_f32[2] << ")";

	DebugText::GetInstance()->Print(Farstr.str(), 0, 250, 2.0f);

	//�I�u�W�F�N�g�̍��W
	std::ostringstream Objstr;
	Objstr << "ObjPos("
		<< std::fixed << std::setprecision(5)
		<< position_.x << ","
		<< position_.y << ","
		<< position_.z << ")";

	DebugText::GetInstance()->Print(Objstr.str(), 0, 300, 2.0f);
	



	//�v���C���[�̈ړ�
	if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_S) || Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_W))
	{
		
		
		/*if (Input::GetInstance()->PushKey(DIK_RIGHT))
		{
			if (position_.x <= 45)
			{
				position_.x += ReticleMove;
			}
		}

		
		if (Input::GetInstance()->PushKey(DIK_LEFT))
		{
			if (position_.x >= -45)
			{
				position_.x -= ReticleMove;
			}
		}

		

		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			if (position_.y <= 20)
			{
				position_.y += ReticleMove;
			}
		}

		if (Input::GetInstance()->PushKey(DIK_DOWN))
		{
			if (position_.y >= -25)
			{
				position_.y -= ReticleMove;
			}
		}*/


		

		


		/*if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
		{
			
			if (IsButtonPush(ButtonKind::RightButton))
			{
				position_.x += ReticleMove;
			}

			if (IsButtonPush(ButtonKind::LeftButton))
			{
				position_.x -= ReticleMove;
			}

			if (IsButtonPush(ButtonKind::UpButton))
			{
				position_.y += ReticleMove;
			}

			if (IsButtonPush(ButtonKind::DownButton))
			{
				position_.y -= ReticleMove;
			}
		}*/



		//�R���g���[���[�̉������X�V
		UpdateInput();
		//}
	}

	


	////�v���C���[����ʔ��O�ɐi�o���悤�Ƃ���Ƃ��͉����߂�
	////�E��
	//if (position_.x >= 45)
	//{
	//	position_.x = 45;
	//}

	////����
	//if (position_.x <= -45)
	//{
	//	position_.x = -45;
	//}

	////�㑤
	//if (position_.y >= 20)
	//{
	//	position_.y = 20;
	//}

	////����
	//if (position_.y <= -20)
	//{
	//	position_.y = -20;
	//}
}


XMFLOAT3 Reticle::GetWorldPosition()
{
	//���[���h���W������ϐ�
	XMFLOAT3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = position_.x;
	worldPos.y = position_.y;
	worldPos.z = position_.z;

	return worldPos;
}

bool Reticle::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{// nullptr�`�F�b�N
	assert(device);

	Reticle::device = device;
	Reticle::cmdList = cmdList;

	Model::SetDevice(device);

	// �J����������
	Camera::InitializeCamera(window_width, window_height);

	// �p�C�v���C��������
	InitializeGraphicsPipeline();

	// �e�N�X�`���ǂݍ���
	//LoadTexture();


	return true;
}





bool Reticle::InitializeGraphicsPipeline()
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
