#pragma once
#include "Mesh.h"

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>

/// <summary>
/// 3Dモデル
/// </summary>
class Model
{
public:
	// 頂点データ構造体
	//struct VertexPosNormalUvSkin
	//{
	//	DirectX::XMFLOAT3 pos; // xyz座標
	//	DirectX::XMFLOAT3 normal; // 法線ベクトル
	//	DirectX::XMFLOAT2 uv;  // uv座標
	//};

	// 定数バッファ用データ構造体B1
	//struct ConstBufferDataB1
	//{
	//	DirectX::XMFLOAT3 ambient;	//アンビエント項
	//	float pad1;			//パディング
	//	DirectX::XMFLOAT3 diffuse;	//ディフューズ係数
	//	float pad2;			//パディング
	//	DirectX::XMFLOAT3 specular;	//スペキュラー係数
	//	float alpha;		//アルファ
	//};

	//マテリアル
	//struct Material
	//{
	//	std::string name;	//マテリアル名
	//	DirectX::XMFLOAT3 ambient;	//アンビエント影響度
	//	DirectX::XMFLOAT3 diffuse;	//ディフューズ影響度
	//	DirectX::XMFLOAT3 specular;	//スペキュラー影響度
	//	float alpha;	//アルファ
	//	std::string textureFilename;	//テクスチャファイル名
	//	//コンストラクタ
	//	Material()
	//	{
	//		ambient = { 0.3f, 0.3f, 0.3f };
	//		diffuse = { 0.0f, 0.0f, 0.0f };
	//		specular = { 0.0f, 0.0f, 0.0f };
	//		alpha = 1.0f;
	//	}
	//};

public:
	//デストラクタ
	~Model();

	//静的初期化
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// OBJファイルから3Dモデルを読み込む
	/// </summary>
	/// <param name="modelname">モデル名</param>
	/// <returns>モデル</returns>
	static Model* LoadFromObj(const std::string& modelname);

	/// <summary>
/// マテリアル読み込み
/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	//マテリアル登録
	void AddMaterial(Material* material);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	void LoadTexture();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootParamIndexMaterial"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

	//メッシュコンテナを取得
	inline const std::vector<Mesh*>& GetMeshs() { return meshes; }

	static void SetDevice(ID3D12Device* device) { Model::device = device; }


private:
	static const std::string baseDirectory;

private:
	//デバイス(借りてくる)
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize_;

	//// 頂点データ配列
	//std::vector<VertexPosNormalUvSkin> vertices_;
	//// 頂点インデックス配列
	//std::vector<unsigned short> indices_;
	//マテリアル
	/*Material material;*/
	// テクスチャバッファ
//	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff_;
	//// デスクリプタヒープ
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap_;
	//// シェーダリソースビューのハンドル(CPU)
	//CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	//// シェーダリソースビューのハンドル(CPU)
	//CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;

	//// 頂点バッファ
	//Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	//// インデックスバッファ
	//Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff__;
	//// 頂点バッファビュー
	//D3D12_VERTEX_BUFFER_VIEW vbView_;
	//// インデックスバッファビュー
	//D3D12_INDEX_BUFFER_VIEW ibView_;
	////定数バッファ
	//Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_; // 定数バッファ

	//名前
	std::string name;
	//メッシュコンテナ
	std::vector<Mesh*> meshes;
	//マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials;
	//デフォルトマテリアル
	Material* defaultMaterial = nullptr;
	//デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

private:
	void Initialize(const std::string& modelname);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	void InitializeDescriptorHeap();

	///各種バッファの生成
	void CreateBuffers();

};

