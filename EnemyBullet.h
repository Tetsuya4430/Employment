#pragma once
#include "Model.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"Input.h"

//自機クラスの前方宣言
class Player;

class EnemyBullet
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


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
	static std::unique_ptr<EnemyBullet>Create(Model* model, Camera* camera, XMFLOAT3 pos, XMFLOAT3 PlayerPos);

public:

	//インスタンス
	static EnemyBullet* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(XMFLOAT3 pos, XMFLOAT3 PlayerPos);

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
	void Update(XMFLOAT3 PlayerPos, XMFLOAT3 EnemyPos);

	//衝突検知コールバック関数
	void OnCollision();

	/// <summary>
/// 静的初期化
/// </summary>
/// <param name="device">デバイス</param>
///<param name="cmdList">描画コマンドリスト</param>
/// <param name="window_width">画面幅</param>
/// <param name="window_height">画面高さ</param>
/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	//デスフラグのゲッター
	bool DeathGetter() const { return DeathFlag; }

	// 座標の取得
	const XMFLOAT3& GetPosition() { return position_B; }

	/// <summary>
	/// setter
	/// </summary>
	void SetModel(Model* model) { model_ = model; }

	void SetCamera(Camera* camera) { camera_ = camera; }

	//プレイヤーのセッター
	void SetPlayer(Player* player) { player_ = player; }

private: // 静的メンバ変数
// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	//弾の寿命
	static const int LifeTimer = 60 * 2;

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
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld_;
	// 親オブジェクト
	EnemyBullet* parent_ = nullptr;

	Input* input = nullptr;

	//プレイヤー
	Player* player_ = nullptr;

	//速度
	XMFLOAT3 Speed = {0.0f, 0.0f, 0.0f};

	//移動用ベクトル
	XMFLOAT3 Vec = {0.0f, 0.0f, 0.0f};

	//弾のデスタイマー
	int DeathTimer = LifeTimer;

	//自機狙い弾用
	float PointPos = 0;
	float Count = 80.0f;

public:
	// ローカル座標
	XMFLOAT3 position_B = { 0,0,0 };

	//弾のデスフラグ
	bool DeathFlag = false;
};

