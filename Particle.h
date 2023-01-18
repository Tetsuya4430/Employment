#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "Camera.h"

class Particle
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;	//xyz���W
		XMFLOAT3 normal; //�@���x�N�g��
		XMFLOAT2 uv;	//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color;	//�F(RGBA)
		XMMATRIX mat;	//3D�ϊ��s��
	};

private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	static const int vertexCount = 4;		// ���_��
	static const int indexCount = 3 * 2;	//�C���f�b�N�X��


public: // �ÓI�����o�֐�
/// <summary>
/// �ÓI������
/// </summary>
/// <param name="device">�f�o�C�X</param>
///<param name="cmdList">�`��R�}���h���X�g</param>
/// <param name="window_width">��ʕ�</param>
/// <param name="window_height">��ʍ���</param>
/// <returns>����</returns>
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);


	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Particle* Create(Camera* camera);

private: // �ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	static ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff;
	//�V�F�[�_�[���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//�V�F�[�_�[���\�[�X�r���[�̃n���h��(GPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//�r���[�s��
	static XMMATRIX matView;
	//�ˉe�ϊ��s��
	static XMMATRIX matProjection;
	//���_���W
	static XMFLOAT3 eye;
	//�����_���W
	static XMFLOAT3 target;
	//������x�N�g��
	static XMFLOAT3 up;
	//���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//�C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;
	//���_�f�[�^�z��
	static VertexPosNormalUv vertices[vertexCount];
	//���_�C���f�b�N�X�z��
	static unsigned short indices[indexCount];

	private:// �ÓI�����o�֐�
	//�f�X�N���v�^�q�[�v�̏�����
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static bool InitializeGraphicsPipeline();

	//�e�N�X�`���ǂݍ���
	static bool LoadTexture();

	/// <summary>
	/// ���f���쐬
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	//static void UpdateViewMatrix();

public: //�����o�֐�
	bool Initialize();

	/// <summary>
/// ���t���[������
/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return position_; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { this->position_ = position; }

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetRotation() { return rotation_; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation_ = rotation; }

	void SetScale(XMFLOAT3 scale) { this->scale_ = scale; }


	void SetCamera(Camera* camera) { camera_ = camera; }

private:	//�����o�ϐ�
	//�s��p�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�J����
	Camera* camera_ = nullptr;
	//�F
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position_ = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld_;
	// �e�I�u�W�F�N�g
	Particle* parent_ = nullptr;

};

