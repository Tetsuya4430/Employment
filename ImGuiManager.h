#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"

//Imguiの管理
class ImGuiManager
{
public:
//メンバ関数

	//インスタンス生成
	static ImGuiManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受け付け開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	void Draw()
		;
//メンバ変数

	//WindowsAPI
	WinApp* winApp = nullptr;
	//dxCommon
	DirectXCommon* dxCommon = nullptr;

	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

