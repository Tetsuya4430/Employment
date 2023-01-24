#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "Camera.h"

class Particle
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//サブクラス

	//頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos;	//x,y,z座標
		float scale;	//スケール
	};

	//パーティクル1粒
	struct Particle_
	{
		//DirectX::を省略
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//座標
		XMFLOAT3 position = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
		//スケール
		float scale = 1.0f;
		//スケール初期値
		float scale_start = 1.0f;
		//スケール最終値
		float scale_end = 0.0f;
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;
	};

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	//3D変換行列
		XMMATRIX matBillboard;	//ビルボード行列
	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = 1024;		// 頂点数

	//テクスチャの最大枚数
	static const int spriteSRVCount = 512;

public: // 静的メンバ関数
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
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Particle* Create(const wchar_t* filename, Camera* camera);

private: // 静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	//テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff;
	//シェーダーリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//シェーダーリソースビューのハンドル(GPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//ビュー行列
	static XMMATRIX matView;
	//射影変換行列
	static XMMATRIX matProjection;
	//視点座標
	static XMFLOAT3 eye;
	//注視点座標
	static XMFLOAT3 target;
	//上方向ベクトル
	static XMFLOAT3 up;
	//頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//頂点データ配列
	static VertexPos vertices[vertexCount];
	//パーティクル配列
	std::forward_list<Particle_> particles;


	private:// 静的メンバ関数
	//デスクリプタヒープの初期化
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	//テクスチャ読み込み
	static bool LoadTexture(const wchar_t* filename);

	/// <summary>
	/// モデル作成
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	//static void UpdateViewMatrix();

public: //メンバ関数
	bool Initialize(const wchar_t* filename);

	/// <summary>
/// 毎フレーム処理
/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position_ = position; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetRotation() { return rotation_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation_ = rotation; }

	void SetScale(XMFLOAT3 scale) { this->scale_ = scale; }


	void SetCamera(Camera* camera) { camera_ = camera; }

	//パーティクル追加
	void AddParticle(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);

	//パーティクル発生関数
	void CreateParticleInfo(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale);

private:	//メンバ変数
	//行列用定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//カメラ
	Camera* camera_ = nullptr;
	//色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// ローカルスケール
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld_;

};

