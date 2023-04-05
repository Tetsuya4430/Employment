#pragma once
#include <windows.h>
#include <wrl.h>
#include "WinApp.h"

#define DIRECTINPUT_VERSION 0x0800	//DirectIput�̃o�[�W�����w��
#include <dinput.h>
class Mouse
{
public:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	//�����o�֐�

	static Mouse* GetInstance();
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();
	//�L�[�̉������`�F�b�N
	bool PushKey(BYTE KeyNumber);
	//�L�[�̃g���K�[���`�F�b�N
	bool TriggerKey(BYTE KeyNumber);

private:	//�����o�ϐ�
	//�L�[�{�[�h�f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//DirectInput�̃C���X�^���X
	ComPtr<IDirectInput8> dinput;
	//�S�L�[�̏��
	DIMOUSESTATE rgbButton[3] = {};
	//�O��̑S�L�[�̏��
	DIMOUSESTATE OldrgbButton[3] = {};
	//WindowsAPI
	WinApp* winApp = nullptr;
};

