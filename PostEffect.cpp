#include "PostEffect.h"
#include"WinApp.h"
#include <d3dx12.h>

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12RootSignature> PostEffect::rootSignature;
ComPtr<ID3D12PipelineState> PostEffect::pipelineState;

PostEffect::PostEffect(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
	:Sprite(
		100,	//�e�N�X�`���ԍ�
		{0, 0},	//�A���J�[�|�C���g
		false,	//���E���]�t���O
		false	//�㉺���]�t���O
	)
{
}

void PostEffect::Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;

	//���N���X�Ƃ��Ă̏�����
	//Sprite;; Initialize(texNumber, anchorpoint, isFlipX, isFlipY);


	//�����o�ϐ��ɏ�������
	texNumber_ = texNumber;
	anchorpoint_ = anchorpoint;
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//���_�f�[�^
	VertexPosUv vertices[] = {
		{{0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},//����
		{{0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},//����
		{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},//�E��
		{{100.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},//�E��
	};

	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	//if (spriteCommon->texBuff_[texNumber_])
	if (spriteCommon->GetTexBuff(texNumber))
	{
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(texNumber)->GetDesc();

		//�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		size_ = { (float)resDesc.Width, (float)resDesc.Height };
		texSize_ = { (float)resDesc.Width, (float)resDesc.Height };
	}

	//���_�o�b�t�@����
	result = spriteCommon->GetDevice_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

	//---���_�o�b�t�@�ւ̃f�[�^�]��---//
	//VertexPosUv* vertMap = nullptr;
	//result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	//memcpy(vertMap, vertices, sizeof(vertices));
	//vertBuff_->Unmap(0, nullptr);

	TransferVertexBuffer();

	//���_�o�b�t�@�r���[�̐���
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices);
	vbView_.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐���
	result = spriteCommon->GetDevice_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);

	//�萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);		//�F�w��(RGBA)
	constMap->mat = spriteCommon->GetmatProjection();
	constBuff_->Unmap(0, nullptr);

	//�e�N�X�`�����\�[�X��ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

	//�e�N�X�`���o�b�t�@�̐���
	result = spriteCommon->GetDevice_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));
		assert(SUCCEEDED(result));
		{
			//�e�N�X�`���N���A
			//��f��(1280x780 = 921600�s�N�Z��)
			const UINT pixelCount = WinApp::window_width * WinApp::window_height;

			//�摜1�s���̃f�[�^�T�C�Y
			const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

			//�摜�S�̂̃f�[�^�T�C�Y
			const UINT depthPitch = rowPitch * WinApp::window_height;

			//�摜�C���[�W
			UINT* img = new UINT[pixelCount];
			for (int i = 0; i < pixelCount; i++)
			{
				img[i] = 0xff0000ff;
			}

			//�e�N�X�`���o�b�t�@�Ƀf�[�^��]��
			result = texBuff->WriteToSubresource(0, nullptr,
				img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[]img;
		}

		//SRV�p�f�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
		srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvDescHeapDesc.NumDescriptors = 1;

		//SRV�p�f�X�N���v�^�q�[�v����
		result = spriteCommon->GetDevice_()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
		assert(SUCCEEDED(result));

		//SRV�ݒ�
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		//�f�X�N���v�^�q�[�v��SRV�쐬
		spriteCommon->GetDevice_()->CreateShaderResourceView(texBuff.Get(),
			&srvDesc,
			descHeapSRV->GetCPUDescriptorHandleForHeapStart()
			);
}

PostEffect* PostEffect::Create(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	//�������m��
	PostEffect* instance = new PostEffect(texNumber, anchorpoint, isFlipX, isFlipY);
	//�C���X�^���X������
	instance->Initialize(texNumber, anchorpoint, isFlipX, isFlipY);

	return instance;
}

void PostEffect::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//��\���t���O��true�Ȃ�
	if (isInvisible_)
	{
		//�`�悹���ɔ�����
		return;
	}

	ID3D12GraphicsCommandList* commandList = spriteCommon->GetCommandList();

	////�p�C�v���C���X�e�[�g�̐ݒ�
	//commandList->SetPipelineState(pipelineState.Get());

	////���[�g�V�O�l�`���ݒ�
	//commandList->SetGraphicsRootSignature(rootSignature.Get());

	////�v���~�e�B�u�`���ݒ�
	//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�o�b�t�@�̃Z�b�g
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
	commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	//���[�g�p�����[�^1�ԂɃV�F�[�_�[���\�[�X�r���[���Z�b�g
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber_);

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	commandList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//�|���S���̕`��
	commandList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDraw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = spriteCommon->GetCommandList();

	////�p�C�v���C���X�e�[�g�̐ݒ�
	//commandList->SetPipelineState(pipelineSet_.pipelinestate.Get());

	////���[�g�V�O�l�`���̐ݒ�
	//commandList->SetGraphicsRootSignature(pipelineSet_.rootsignature.Get());

	////�v���~�e�B�u�`���ݒ�
	//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	////�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//commandList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());


}
