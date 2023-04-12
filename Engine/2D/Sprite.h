#pragma once

#include "SpriteCommon.h"

#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

/// <summary>
/// �X�v���C�g1������\���N���X
/// </summary>
class Sprite
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	//�X�v���C�g�p�\����
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos;	//xyz���W
		DirectX::XMFLOAT2 uv;	//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;	//�F(R,G,B,A)
		DirectX::XMMATRIX mat;	//3D�ϊ��s��
	};

public:
	Sprite(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="spriteCommon">�X�v���C�g����</param>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">X���]���邩</param>
	/// <param name="isFlipY">Y���]���邩</param>
	static Sprite* Create(UINT texNumber, XMFLOAT4 color, DirectX::XMFLOAT2 anchorpoint = {1.0f, 1.0f}, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(UINT texNumber, XMFLOAT4 color_, DirectX::XMFLOAT2 anchorpoint , bool isFlipX, bool isFlipY);

	/// <summary>
	/// ���_�o�b�t�@�̓]��
	/// </summary>
	void TransferVertexBuffer();


	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���t���[���X�V����
	/// </summary>
	void Update();

	void SetPosition(const DirectX::XMFLOAT3& position) { position_ = position; }
	void SetRotation(float rotation) { rotation_ = rotation; }
	void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; }
	void SetTexLeftTop(const DirectX::XMFLOAT2& texLeftTop) { texLeftTop_ = texLeftTop; }
	void SetTexSize(const DirectX::XMFLOAT2& texSize) { texSize_ = texSize; }
	void SetColor(const DirectX::XMFLOAT4& color) { color_ = color; }

	//getter
	// ���W�̎擾
	const XMFLOAT2& GetSize() { return size_; }


	protected:
	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	//z���܂��̉�]�p
	float rotation_ = 0.0f;

	//���[���h���W
	DirectX::XMMATRIX matWorld_;


	//�e�N�X�`���ԍ�
	UINT texNumber_ = 0;

	//�傫��
	DirectX::XMFLOAT2 size_ = { 100, 100};

	//�A���J�[�|�C���g
	DirectX::XMFLOAT2 anchorpoint_ = { 1.0f, 1.0f };

	//���E���]
	bool isFlipX_ = false;

	//�㉺���]
	bool isFlipY_ = false;

	//�e�N�X�`��������W
	DirectX::XMFLOAT2 texLeftTop_ = { 0, 0 };

	//�e�N�X�`���؂�o���T�C�Y
	DirectX::XMFLOAT2 texSize_ = { 100, 100 };

	//��\��
	bool isInvisible_ = false;

	

	public:
	//���W
	DirectX::XMFLOAT3 position_ = { 0, 0, 0 };
	//�T�C�Y�ύX�p�ϐ�
	XMFLOAT2 BarSize = { 100, 200 };

	// �F(RGBA)
	DirectX::XMFLOAT4 color_ = { 1, 1, 1, 1 };

	//�C�[�W���O�p�ϐ�
	XMFLOAT2 Vec = { 0, 0 };
	XMFLOAT2 PointPos ={0, 0};

	//�ړ������t���O
	bool ComFlag = false;
	bool ComFlag_2 = false;
};
