#pragma once

#include "SpriteCommon.h"

#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

#include <memory>

/// <summary>
/// スプライト1枚分を表すクラス
/// </summary>
class Sprite
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
	//スプライト用構造体
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos;	//xyz座標
		DirectX::XMFLOAT2 uv;	//uv座標
	};

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;	//色(R,G,B,A)
		DirectX::XMMATRIX mat;	//3D変換行列
	};

public:
	Sprite(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="spriteCommon">スプライト共通</param>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">X反転するか</param>
	/// <param name="isFlipY">Y反転するか</param>
	static std::unique_ptr <Sprite> Create(UINT texNumber, XMFLOAT4 color, DirectX::XMFLOAT2 anchorpoint = {1.0f, 1.0f}, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(UINT texNumber, XMFLOAT4 color_, DirectX::XMFLOAT2 anchorpoint , bool isFlipX, bool isFlipY);

	/// <summary>
	/// 頂点バッファの転送
	/// </summary>
	void TransferVertexBuffer();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 毎フレーム更新処理
	/// </summary>
	void Update();

	void SetPosition(const DirectX::XMFLOAT3& position) { position_ = position; }
	void SetRotation(float rotation) { rotation_ = rotation; }
	void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; }
	void SetTexLeftTop(const DirectX::XMFLOAT2& texLeftTop) { texLeftTop_ = texLeftTop; }
	void SetTexSize(const DirectX::XMFLOAT2& texSize) { texSize_ = texSize; }
	void SetColor(const DirectX::XMFLOAT4& color) { color_ = color; }

	//getter
	// 座標の取得
	const XMFLOAT2& GetSize() { return size_; }


	protected:
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	//z軸まわりの回転角
	float rotation_ = 0.0f;

	//ワールド座標
	DirectX::XMMATRIX matWorld_;


	//テクスチャ番号
	UINT texNumber_ = 0;

	//大きさ
	DirectX::XMFLOAT2 size_ = { 100, 100};

	//アンカーポイント
	DirectX::XMFLOAT2 anchorpoint_ = { 1.0f, 1.0f };

	//左右反転
	bool isFlipX_ = false;

	//上下反転
	bool isFlipY_ = false;

	//テクスチャ左上座標
	DirectX::XMFLOAT2 texLeftTop_ = { 0, 0 };

	//テクスチャ切り出しサイズ
	DirectX::XMFLOAT2 texSize_ = { 100, 100 };

	//非表示
	bool isInvisible_ = false;

	

	public:
	//座標
	DirectX::XMFLOAT3 position_ = { 0, 0, 0 };
	//サイズ変更用変数
	XMFLOAT2 BarSize = { 100, 200 };

	// 色(RGBA)
	DirectX::XMFLOAT4 color_ = { 1, 1, 1, 1 };

	//イージング用変数
	XMFLOAT2 Vec = { 0, 0 };
	XMFLOAT2 PointPos ={0, 0};

	//移動完了フラグ
	bool ComFlag = false;
	bool ComFlag_2 = false;
};

