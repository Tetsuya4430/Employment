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

