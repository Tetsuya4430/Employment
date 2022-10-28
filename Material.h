#pragma once


#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
class Material
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//�萔�o�b�t�@�p�\����
	struct ConstBufferData //���O�ς��邩��
	{
		XMFLOAT3 ambient;	//�A���r�G���g�e���x
		float pad1;
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		float pad2;
		XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;	//�A���t�@
		//std::string textureFilename;	//�e�N�X�`���t�@�C����
	};

	//�ÓI�����o�֐�
public:
	//�ÓI������
	static void StaticInitialize(ID3D12Device* device);

	//�}�e���A������
	static Material* Create();

private:	//�ÓI�����o�ϐ�
	static ID3D12Device* device;

public:
	std::string name;	//�}�e���A����
	XMFLOAT3 ambient;	//�A���r�G���g�e���x
	XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
	XMFLOAT3 specular;	//�X�y�L�����[�e���x
	float alpha;		//�A���t�@
	std::string textureFileName;	//�e�N�X�`���t�@�C����

public:

	//�萔�o�b�t�@�̎擾
	ID3D12Resource* GetConstBuffer() { return constBuff.Get(); }

	//�e�N�X�`���ǂݍ���
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);

	//�X�V
	void Update();

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV; }

private:
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�V�F�[�_�[���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//�V�F�[�_�[���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

private:
	//�R���X�g���N�^
	Material()
	{
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	//������
	void Initialize();

	//�萔�o�b�t�@�̐���
	void CreateConstantBuffer();
};
