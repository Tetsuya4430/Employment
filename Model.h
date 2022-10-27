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

