#pragma once
#include <windows.h>
#include <wrl.h>
#include "WinApp.h"

#define DIRECTINPUT_VERSION 0x0800	//DirectIputのバージョン指定
#include <dinput.h>
class Mouse
{
public:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	//メンバ関数

	static Mouse* GetInstance();
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();
	//キーの押下をチェック
	bool PushKey(BYTE KeyNumber);
	//キーのトリガーをチェック
	bool TriggerKey(BYTE KeyNumber);

private:	//メンバ変数
	//キーボードデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput;
	//全キーの状態
	DIMOUSESTATE rgbButton[3] = {};
	//前回の全キーの状態
	DIMOUSESTATE OldrgbButton[3] = {};
	//WindowsAPI
	WinApp* winApp = nullptr;
};

