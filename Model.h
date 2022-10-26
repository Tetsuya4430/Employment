#pragma once
#include "Mesh.h"

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>

/// <summary>
/// 3D���f��
/// </summary>
class Model
{
public:
	// ���_�f�[�^�\����
	//struct VertexPosNormalUvSkin
	//{
	//	DirectX::XMFLOAT3 pos; // xyz���W
	//	DirectX::XMFLOAT3 normal; // �@���x�N�g��
	//	DirectX::XMFLOAT2 uv;  // uv���W
	//};

	// �萔�o�b�t�@�p�f�[�^�\����B1
	//struct ConstBufferDataB1
	//{
	//	DirectX::XMFLOAT3 ambient;	//�A���r�G���g��
	//	float pad1;			//�p�f�B���O
	//	DirectX::XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
	//	float pad2;			//�p�f�B���O
	//	DirectX::XMFLOAT3 specular;	//�X�y�L�����[�W��
	//	float alpha;		//�A���t�@
	//};

	//�}�e���A��
	//struct Material
	//{
	//	std::string name;	//�}�e���A����
	//	DirectX::XMFLOAT3 ambient;	//�A���r�G���g�e���x
	//	DirectX::XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
	//	DirectX::XMFLOAT3 specular;	//�X�y�L�����[�e���x
	//	float alpha;	//�A���t�@
	//	std::string textureFilename;	//�e�N�X�`���t�@�C����
	//	//�R���X�g���N�^
	//	Material()
	//	{
	//		ambient = { 0.3f, 0.3f, 0.3f };
	//		diffuse = { 0.0f, 0.0f, 0.0f };
	//		specular = { 0.0f, 0.0f, 0.0f };
	//		alpha = 1.0f;
	//	}
	//};

public:
	//�f�X�g���N�^
	~Model();

	//�ÓI������
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// OBJ�t�@�C������3D���f����ǂݍ���
	/// </summary>
	/// <param name="modelname">���f����</param>
	/// <returns>���f��</returns>
	static Model* LoadFromObj(const std::string& modelname);

	/// <summary>
/// �}�e���A���ǂݍ���
/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	//�}�e���A���o�^
	void AddMaterial(Material* material);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	void LoadTexture();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="rootParamIndexMaterial"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

	//���b�V���R���e�i���擾
	inline const std::vector<Mesh*>& GetMeshs() { return meshes; }

	static void SetDevice(ID3D12Device* device) { Model::device = device; }


private:
	static const std::string baseDirectory;

private:
	//�f�o�C�X(�؂�Ă���)
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize_;

	//// ���_�f�[�^�z��
	//std::vector<VertexPosNormalUvSkin> vertices_;
	//// ���_�C���f�b�N�X�z��
	//std::vector<unsigned short> indices_;
	//�}�e���A��
	/*Material material;*/
	// �e�N�X�`���o�b�t�@
//	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff_;
	//// �f�X�N���v�^�q�[�v
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap_;
	//// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	//CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	//// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	//CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

	//// ���_�o�b�t�@
	//Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	//// �C���f�b�N�X�o�b�t�@
	//Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff__;
	//// ���_�o�b�t�@�r���[
	//D3D12_VERTEX_BUFFER_VIEW vbView_;
	//// �C���f�b�N�X�o�b�t�@�r���[
	//D3D12_INDEX_BUFFER_VIEW ibView_;
	////�萔�o�b�t�@
	//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_; // �萔�o�b�t�@

	//���O
	std::string name;
	//���b�V���R���e�i
	std::vector<Mesh*> meshes;
	//�}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials;
	//�f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;
	//�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

private:
	void Initialize(const std::string& modelname);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	void InitializeDescriptorHeap();

	///�e��o�b�t�@�̐���
	void CreateBuffers();

};

