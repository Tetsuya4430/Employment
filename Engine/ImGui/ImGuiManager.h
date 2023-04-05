#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"

//Imgui�̊Ǘ�
class ImGuiManager
{
public:
//�����o�֐�

	//�C���X�^���X����
	static ImGuiManager* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui�󂯕t���J�n
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui��t�I��
	/// </summary>
	void End();

	/// <summary>
	/// ��ʂւ̕`��
	/// </summary>
	void Draw()
		;
//�����o�ϐ�

	//WindowsAPI
	WinApp* winApp = nullptr;
	//dxCommon
	DirectXCommon* dxCommon = nullptr;

	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

