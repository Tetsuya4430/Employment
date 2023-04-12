#pragma once
#include <DirectXMath.h>

#include "Input.h"

class Camera
{
private: // �G�C���A�X 
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:	//�����o�֐�
	
	//�C���X�^���X
	static Camera* GetInstance();

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveVector(XMFLOAT3 move);

	static void CameraMoveEyeVector(XMFLOAT3 move);

	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();


	void CameraShake(XMFLOAT3 move);

	void EyeReset();

	/// <summary>
	/// �ˉe�s��̎擾
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	inline const XMMATRIX& GetProjectionMatrix() {
		return matProjection;
	}

	/// <summary>
	/// �r���[�ˉe�s��̎擾
	/// </summary>
	/// <returns>�r���[�ˉe�s��</returns>
	inline const XMMATRIX& GetViewProjectionMatrix() {
		return matViewProjection;
	}

	//�r���[�ˉe�s��̎擾
	static const XMMATRIX& GetmatViewProjection() { return matViewProjection; }

	//�r���{�[�h�̎擾
	static const XMMATRIX& GetBillBoard() { return matBillboard; }

	static const XMMATRIX& GetBillBoardY() { return matBillboardY; }

	static void Update();


private:	//�����o�ϐ�
	//�r���[�ˉe�s��
	static XMMATRIX matViewProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;

	//�f�t�H���g�̃J�������W
	static XMFLOAT3 DefaultEyePos;




	//�C���v�b�g
	Input* input = nullptr;

	int ShakeTimer = 0;

public:
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	//�r���{�[�h�s��
	static XMMATRIX matBillboard;
	//Y�����r���{�[�h�s��
	static XMMATRIX matBillboardY;

	XMFLOAT3 moveVec = { 0.0f, 0.0f, 0.0f };
};