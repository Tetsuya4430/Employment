#pragma once
#include "Model.h"
#include "Camera.h"
#include "Bullet.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"Input.h"


class Player 
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;


public:
	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static Player* Create(Model* model, Camera* camera);

public:

	//�C���X�^���X
	static Player* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// �I������
	/// </summary>
	/// <returns></returns>
	bool Finalize();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	//�Փˌ��m�R�[���o�b�N�֐�
	void OnCollision();

	//���[���h���W���擾
	XMFLOAT3 GetWorldPosition();

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
	/// setter
	/// </summary>
	void SetModel(Model* model) { model_ = model; }

	void SetCamera(Camera* camera) { camera_ = camera; }

/// <summary>
/// getter
/// </summary>

	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position_; }

	//�e���X�g���擾
	//const std::list<std::unique_ptr<Bullet>>& GetBullets() {return bullets_}

private: // �ÓI�����o�ϐ�
// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static bool InitializeGraphicsPipeline();

	private: // �����o�ϐ�
//3D���f��(�؂�Ă���)
	Model* model_ = nullptr;
	//�J����
	Camera* camera_ = nullptr;
	//�s��p�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0_;
	// ���[�J���X�P�[��
	XMFLOAT3 scale_ = {1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld_;
	// �e�I�u�W�F�N�g
	Player* parent_ = nullptr;

	Input* input = nullptr;

	

	//�v���C���[��]���x
	float RotlimR = -15.0f;
	float RotlimL = 15.0f;

	//�v���C���[�̈ړ��t���O
	int MoveFlag = 0;

	public:
		// ���[�J�����W
		XMFLOAT3 position_ = { 0,0,0 };

		//�v���C���[��HP
		int HP = 5;
};

