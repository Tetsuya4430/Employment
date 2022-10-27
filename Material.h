#pragma once


#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
class Material
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//定数バッファ用構造体
	struct ConstBufferData //名前変えるかも
	{
		XMFLOAT3 ambient;	//アンビエント影響度
		float pad1;
		XMFLOAT3 diffuse;	//ディフューズ影響度
		float pad2;
		XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;	//アルファ
		//std::string textureFilename;	//テクスチャファイル名
	};

	//静的メンバ関数
public:
	//静的初期化
	static void StaticInitialize(ID3D12Device* device);

	//マテリアル生成
	static Material* Create();

private:	//静的メンバ変数
	static ID3D12Device* device;

public:
	std::string name;	//マテリアル名
	XMFLOAT3 ambient;	//アンビエント影響度
	XMFLOAT3 diffuse;	//ディフューズ影響度
	XMFLOAT3 specular;	//スペキュラー影響度
	float alpha;		//アルファ
	std::string textureFileName;	//テクスチャファイル名

public:

	//定数バッファの取得
	ID3D12Resource* GetConstBuffer() { return constBuff.Get(); }

	//テクスチャ読み込み
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);

	//更新
	void Update();

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV; }

private:
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//シェーダーリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//シェーダーリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

private:
	//コンストラクタ
	Material()
	{
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	//初期化
	void Initialize();

	//定数バッファの生成
	void CreateConstantBuffer();
};

