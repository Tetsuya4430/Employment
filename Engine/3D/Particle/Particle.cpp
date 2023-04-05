#include "Particle.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

//�ÓI�����o�ϐ��̎���
const float Particle::radius = 5.0f;
const float Particle::prizmHeight = 8.0f;
ID3D12Device* Particle::device = nullptr;
UINT Particle::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* Particle::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Particle::rootsignature;
ComPtr<ID3D12PipelineState> Particle::pipelinestate;
ComPtr<ID3D12DescriptorHeap> Particle::descHeap;
ComPtr<ID3D12Resource> Particle::vertBuff;
ComPtr<ID3D12Resource> Particle::texBuff[MaxTexture];
CD3DX12_CPU_DESCRIPTOR_HANDLE Particle::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE Particle::gpuDescHandleSRV;
XMMATRIX Particle::matView{};
XMMATRIX matProjection{};
XMFLOAT3 Particle::eye = { 0, 0, -50.0f };
XMFLOAT3 Particle::target = { 0, 0, 0 };
XMFLOAT3 Particle::up = { 0, 1, 0};
D3D12_VERTEX_BUFFER_VIEW Particle::vbView{};
Particle::VertexPos Particle::vertices[vertexCount];

//XMFLOAT3���m�̉��Z����
const DirectX::XMFLOAT3 operator+(const::DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;

	return result;
}

Particle* Particle::GetInstance()
{
	static Particle instance;

	return &instance;
}

bool Particle::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{
	// nullptr�`�F�b�N
	assert(device);

	Particle::device = device;
	Particle::cmdList = cmdList;

	//�f�X�N���v�^�q�[�v�̏�����
	InitializeDescriptorHeap();

	//�J����������
	Camera::InitializeCamera(window_width, window_height);

	//�p�C�v���C��������
	InitializeGraphicsPipeline();

	//���f������
	CreateModel();


	return true;
}

void Particle::PreDraw()
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Particle::PostDraw()
{
	//�R�}���h���X�g������
	//Particle::cmdList = nullptr;
}

Particle* Particle::Create(UINT texnumber, Camera* camera)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Particle* particle = new Particle();
	if (particle == nullptr) {
		return nullptr;
	}

	// ������
	if (!particle->Initialize(texnumber)) {
		delete particle;
		assert(0);
		return nullptr;
	}

	if (camera)
	{
		particle->SetCamera(camera);
	}

	return particle;
}

void Particle::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texnumber)
{
	cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texnumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
}

bool Particle::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = MaxTexture;	//�V�F�[�_�[���\�[�X�r���[1��
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	//�f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool Particle::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob; //�W�I���g���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleVS.hlsl",	// �V�F�[�_�t�@�C����
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

	// �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleGS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);
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
		L"Resources/shaders/ParticlePS.hlsl",	// �V�F�[�_�t�@�C����
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

		{//�X�P�[��
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{//�F
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{//��]
			"ROTATION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//�f�v�X�̏������݋֎~
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	//����������
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//���Z����
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	//���Z����
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/

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
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

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

void Particle::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	HRESULT result;

	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0);	//���f�[�^���o

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	//�e�N�X�`���o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texBuff[texnumber] ));

	

	//�e�N�X�`���o�b�t�@�ɓ]��
	result = texBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,		//1���C���T�C�Y
		(UINT)img->slicePitch	//�S�T�C�Y
	);


	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	//cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	//gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	//D3D12_RESOURCE_DESC resDesc = texBuff[texnumber]->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v��texnumber�ɃV�F�[�_�[���\�[�X�r���[���쐬
	device->CreateShaderResourceView(texBuff[texnumber].Get(),		//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,	//�e�N�X�`�����
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);

}

void Particle::CreateModel()
{
	HRESULT result = S_FALSE;

	std::vector<VertexPos> realVertices;
	
	////�l�p�`�̒��_�f�[�^
	//VertexPos verticesPoint[] = {
	//	{{0.0f, 0.0f, 0.0f}}
	//};

	////�����o�ϐ��ɃR�s�[
	//std::copy(std::begin(verticesPoint), std::end(verticesPoint), vertices);

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}

bool Particle::Initialize(UINT texnumber)
{
	// nullptr�`�F�b�N
	assert(device);

	//�e�N�X�`���ԍ�����������
	this->texNumber = texnumber;

	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (texBuff[texNumber])
	{
		//�e�N�X�`�������擾
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
	}

	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	//�e�N�X�`���ǂݍ���
	//LoadTexture(texnumber, filename);

	return true;
}

void Particle::Update(/*XMFLOAT4 color*/)
{
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
	//matWorld_ *= camera_->GetBillBoardY();
	matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	//�������s�����p�[�e�B�N����S�폜
	particles.remove_if([](Particle_& x)
		{
			return x.frame >= x.num_frame;
		}
	);

	//�S�p�[�e�B�N���X�V
	for (std::forward_list<Particle_>::iterator it = particles.begin();
		it != particles.end(); it++)
	{
		//�o�߃t���[�������J�E���g
		it->frame++;
		//�i�s�x��0�`1�͈̔͂Ɋ��Z
		float f = (float)it->num_frame / it->frame;
		//���x�ɉ����x�����Z
		it ->velocity = it->velocity + it->accel;
		//���x�ɂ��ړ�
		it->position = it->position + it->velocity;
		//�X�P�[���̐��`���
		it->scale = (it->scale_end - it->scale_start) / f;
		it->scale += it->scale_start;
		//�F�̒l��n��
		it->color = it->color;
		//��]�̒l��n��
		it->rotation = rotation_;
	}

	//���_�o�b�t�@�փf�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	if (SUCCEEDED(result))
	{
		//�p�[�e�B�N���̏���1�����f
		for (std::forward_list<Particle_>::iterator it = particles.begin();
			it != particles.end(); it++)
		{
			//���W
			vertMap->pos = it->position;
			//�X�P�[��
			vertMap->scale = it->scale;
			//�F
			vertMap->color = it->color;
			//��]
			vertMap->rotation = it->rotation;
			//���̒��_��
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}

	const XMMATRIX& matViewProjection = camera_->GetmatViewProjection();

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color_;
	constMap->mat = matWorld_ * matViewProjection;	// �s��̍���
	constMap->matBillboard = camera_->matBillboard;
	constBuff->Unmap(0, nullptr);
}

void Particle::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(Particle::cmdList);

	//���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	////�C���f�b�N�X�o�b�t�@�̐ݒ�
	//cmdList->IASetIndexBuffer(&ibView);

	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	SetGraphicsRootDescriptorTable(1, texNumber/*gpuDescHandleSRV*/);
	//�`��R�}���h
	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

void Particle::AddParticle(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale, XMFLOAT4 color)
{
	//���X�g�ɗv�f��ǉ�
	particles.emplace_front();
	//�ǉ������v�f�̎Q��
	Particle_& p = particles.front();
	//�l�̃Z�b�g
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	//p.scale = start_scale;
	p.scale_start = start_scale;
	p.scale_end = end_scale;
	p.color = color;
}

void Particle::CreateParticleInfo(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z(���W)
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z(���x)
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//�ǉ�
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::DeathParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z(���W)
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z(���x)
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//�ǉ�
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::FireParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT3 Speed, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//�����̒l��Ⴄ
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x/* +(float)rand() / RAND_MAX * md_width - md_width/ 2.0f*/;
		pos.y = Position.y/* + (float)rand() / RAND_MAX * md_width - md_width / 2.0f*/;
		pos.z = Position.z + 10;
		//�p�[�e�B�N���̑��x
		const float md_vel = 0.5;
		XMFLOAT3 vel{};
		vel.x = Speed.x;
		vel.y = Speed.y;
		vel.z -= Speed.z / 10;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		//const float md_acc = 0.001f;
		//acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//�ǉ�
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::EnemyFireParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT3 Speed, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//�����̒l��Ⴄ
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//�p�[�e�B�N���̑��x
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.x += Speed.x / 10;
		vel.y += Speed.y / 10;
		vel.z -= Speed.z / 10;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//�ǉ�
		AddParticle(ParticleLife, pos, vel, rot, StartScale, EndScale, color);

	}
}

void Particle::PlayerLevelUpParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//���S���W
		XMFLOAT3 Center{};
		//���a
		float Radius;
		//�p�x
		float angle;
		//���a�̒���
		float Length;

		//�����̒l��Ⴄ
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//�p�[�e�B�N���̑��x
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = 0.5f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//�ǉ�
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::LevelUpParticle(int PartNum, XMFLOAT3 Position, XMFLOAT3 EndPosition, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//���S���W
		XMFLOAT3 Center{};
		//���a
		float Radius;
		//�p�x
		float angle;
		//���a�̒���
		float Length;

		//�����̒l��Ⴄ
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//�p�[�e�B�N���̑��x
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = 0.5f;
		vel.z = 0;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//�ǉ�
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::JettParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT3 Speed, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//�����̒l��Ⴄ
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x/* +(float)rand() / RAND_MAX * md_width - md_width/ 2.0f*/;
		pos.y = Position.y/* + (float)rand() / RAND_MAX * md_width - md_width / 2.0f*/;
		pos.z = Position.z + 10;
		//�p�[�e�B�N���̑��x
		const float md_vel = 0.2;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = Speed.y;
		vel.z -= Speed.z / 10;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		//const float md_acc = 0.001f;
		//acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//�ǉ�
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::WarningParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT3 Speed, XMFLOAT4 color)
{
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	for (int i = 0; i < PartNum; i++)
	{
		//�����̒l��Ⴄ
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z + 10;
		//�p�[�e�B�N���̑��x
		const float md_vel = 0.5;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.x += Speed.x / 10;;
		vel.y += Speed.y / 10;
		vel.z -= Speed.z / 10;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		//const float md_acc = 0.001f;
		//acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;

		//�ǉ�
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

