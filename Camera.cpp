#include "Camera.h"

using namespace DirectX;

XMMATRIX Camera::matView{};
XMMATRIX Camera::matProjection{};
XMMATRIX Camera::matViewProjection{};
XMFLOAT3 Camera::eye = {70, 0, -50.0f };
XMFLOAT3 Camera::target = { 0, 0, 0 };
XMFLOAT3 Camera::up = { 0, 1, 0 };
XMMATRIX Camera::matBillboard = XMMatrixIdentity();
XMMATRIX Camera::matBillboardY = XMMatrixIdentity();

Camera* Camera::GetInstance()
{
	static Camera instance;

	return &instance;
}

void Camera::SetEye(XMFLOAT3 eye)
{
	Camera::eye = eye;

	UpdateViewMatrix();
}

void Camera::SetTarget(XMFLOAT3 target)
{
	Camera::target = target;

	UpdateViewMatrix();
}

void Camera::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraMoveEyeVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void Camera::InitializeCamera(int window_width, int window_height)
{
	//// �r���[�s��̐���
	//matView = XMMatrixLookAtLH(
	//	XMLoadFloat3(&eye),
	//	XMLoadFloat3(&target),
	//	XMLoadFloat3(&up));

	//�r���[�s��̌v�Z
	UpdateViewMatrix();

	//���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);

	//�����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);

	//(����)�����
	XMVECTOR upVector = XMLoadFloat3(&up);

	//�J����Z��(��������)
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//�x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//�J������X��(�E����)
	XMVECTOR cameraAxisX;

	//X���͏����->Z���̊O�ςŌ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);

	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//�J������Y��(�����)
	XMVECTOR cameraAxisY;

	//Y����Z��->X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//�J������]�s��
	XMMATRIX matCameraRot;

	//�J�������W�n->���[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//�]�u�ɂ��t�s��(�t��])���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	XMVECTOR reverceEyePosition = XMVectorNegate(eyePosition);

	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��(�J�������W)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverceEyePosition);	//X����
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverceEyePosition);	//Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverceEyePosition);	//Z����

	//1�̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//�r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

	//�r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	// ���s���e�ɂ��ˉe�s��̐���
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);

	//�r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}

void Camera::UpdateViewMatrix()
{
	// �r���[�s��̍X�V
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::Update()
{
	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}
