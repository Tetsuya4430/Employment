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
#include "Easing.h"


class Player 
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;


public:
	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// ３Ｄ変換行列
	};

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model"></param>
	/// <param name="camera"></param>
	/// <returns></returns>
	static Player* Create(Model* model, Camera* camera);

public:

	//インスタンス
	static Player* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <returns></returns>
	bool Finalize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	//ワールド座標を取得
	XMFLOAT3 GetWorldPosition();

	/// <summary>
/// 静的初期化
/// </summary>
/// <param name="device">デバイス</param>
///<param name="cmdList">描画コマンドリスト</param>
/// <param name="window_width">画面幅</param>
/// <param name="window_height">画面高さ</param>
/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);



	/// <summary>
	/// setter
	/// </summary>
	void SetModel(Model* model) { model_ = model; }

	void SetCamera(Camera* camera) { camera_ = camera; }

/// <summary>
/// getter
/// </summary>

	// 座標の取得
	const XMFLOAT3& GetPosition() { return position_; }

	//setter

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position_ = position; }

	//大きさの設定
	void SetScale(XMFLOAT3 scale) { this->scale_ = scale; }

	//弾リストを取得
	//const std::list<std::unique_ptr<Bullet>>& GetBullets() {return bullets_}

private: // 静的メンバ変数
// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	private: // メンバ変数
//3Dモデル(借りてくる)
	Model* model_ = nullptr;
	//カメラ
	Camera* camera_ = nullptr;
	//行列用定数バッファ
	ComPtr<ID3D12Resource> constBuffB0_;
	// ローカルスケール
	XMFLOAT3 scale_ = {0.5,0.5,0.5 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld_;
	// 親オブジェクト
	Player* parent_ = nullptr;

	Input* input = nullptr;

	

	//プレイヤー回転限度
	float RotlimR = -15.0f;
	float RotlimL = 15.0f;

	//プレイヤーの移動フラグ
	int MoveFlag = 0;

	//プレイヤーイージング用変数
	float dx = 0;
	float PointPos = -50;

	//回避開始時の自機の座標を格納する変数
	float AvoidStart_X = 0;

	//回避フラグ
	bool Avoidanceflag_X = false;
	bool Avoidanceflag_Y = false;

	//回避タイマー
	int AvoidanceTimer_X = 0;
	int AvoidanceTimer_Y = 0;

	//回避距離定数
	const int AvoidDistance_X = 20;
	const int AvoidDistance_Y = 10;

	//回転距離定数
	const float RoleDistance = 360;

	//回転格納変数
	float RolePos = 0;

	float rx = 0;

	//回避旋回方向フラグ
	bool RotFlag_R = false;
	bool RotFlag_L = false;

	//回避タイム
	const float AvoidCount = 10.0f;

	public:
		// ローカル座標
		XMFLOAT3 position_ = { 0,0,0 };

		//レティクル座標
		XMFLOAT3 ReticlePos = { 0, 0, 0 };

		//プレイヤーのHP
		int HP = 5;
};

