#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Material.h"
#include <vector>
class Mesh
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
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

public:	//�ÓI�����o�֐�
	static void StaticInitialize(ID3D12Device* device);

private:	//�ÓI�����o�ϐ�
	static ID3D12Device* device;

public:		//�����o�֐�

	//���O�̎擾
	const std::string& GetName() { return name; }

	//���O�̃Z�b�g
	void SetName(const std::string& name);

	//���_�f�[�^�̒ǉ�
	void AddVertex(const VertexPosNormalUv& vertex);

	//���_�C���f�b�N�X�̒ǉ�
	void AddIndex(unsigned short index);

	//���_�f�[�^�̐����擾
	inline size_t GetVertexCount() { return vertices.size(); }

	//�}�e���A���̎擾
	Material* GetMaterial() { return material; }

	//�}�e���A���̊��蓖��
	void SetMaterial(Material* material);

	//�o�b�t�@�̐���
	void CreateBuffers();

	//���_�o�b�t�@�̎擾
	const D3D12_VERTEX_BUFFER_VIEW GetVBView() { return vbView; }

	//�C���f�b�N�X�o�b�t�@�̎擾
	const D3D12_INDEX_BUFFER_VIEW GetIBView() { return ibView; }

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���_�z����擾
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices; }

	//�C���f�b�N�X�z����擾
	inline const std::vector<unsigned short>& GetIndices() { return indices; }

private:	//�����o�ϐ�
	
	//���O
	std::string name;
	
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;

	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	//���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;

	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	//�}�e���A��
	Material* material = nullptr;
};
