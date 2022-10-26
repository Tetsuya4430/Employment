#include "Model.h"

#include <string>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>

using namespace DirectX;
using namespace std;

//静的メンバ変数の実体
const std::string Model::baseDirectory = "Resources/";
ID3D12Device* Model::device = nullptr;
UINT Model::descriptorHandleIncrementSize_ = 0;

Model::~Model()
{
	for (auto m : meshes)
	{
		delete m;
	}
	meshes.clear();

	for (auto m : materials)
	{
		delete m.second;
	}
	materials.clear();
}

void Model::StaticInitialize(ID3D12Device* device)
{
	Model::device = device;

	//メッシュの静的初期化
	Mesh::StaticInitialize(device);
}

Model* Model::LoadFromObj(const std::string& modelname)
{
    Model* instance = new Model;

	////デスクリプタヒープ生成
	//instance->InitializeDescriptorHeap();

	//OBJファイルからデータを読み込む
	instance->Initialize(modelname);

	////バッファ生成
	//instance->CreateBuffers();

	return instance;
}

void Model::LoadTexture()
{
	int textureIndex = 0;
	string directoryPath = baseDirectory + name + "/";

	for (auto& m : materials)
	{
		Material* material = m.second;

		//テクスチャあり
		if (material->textureFileName.size() > 0)
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);

			//マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);

			textureIndex++;
		}
		//テクスチャなし
		else
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);

			//マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);

			textureIndex++;
		}
	}

	//HRESULT result = S_FALSE;

	//// WICテクスチャのロード
	//TexMetadata metadata{};
	//ScratchImage scratchImg{};

	////ファイルパス結合
	//std::string filepath = directoyPath + filename;

	////ユニコード文字列に変換する
	//wchar_t wfilepath[128];
	//int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//result = LoadFromWICFile(
	//	wfilepath, WIC_FLAGS_NONE,
	//	&metadata, scratchImg);
	//if (FAILED(result)) {
	//	return result;
	//}

	//const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	//// リソース設定
	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	metadata.format,
	//	metadata.width,
	//	(UINT)metadata.height,
	//	(UINT16)metadata.arraySize,
	//	(UINT16)metadata.mipLevels
	//);

	//// テクスチャ用バッファの生成
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
	//	nullptr,
	//	IID_PPV_ARGS(&texbuff_));
	//if (FAILED(result)) {
	//	return result;
	//}

	//// テクスチャバッファにデータ転送
	//result = texbuff_->WriteToSubresource(
	//	0,
	//	nullptr, // 全領域へコピー
	//	img->pixels,    // 元データアドレス
	//	(UINT)img->rowPitch,  // 1ラインサイズ
	//	(UINT)img->slicePitch // 1枚サイズ
	//);
	//if (FAILED(result)) {
	//	return result;
	//}

	//// シェーダリソースビュー作成
	//cpuDescHandleSRV_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap_->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);
	//gpuDescHandleSRV_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap_->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	//D3D12_RESOURCE_DESC resDesc = texbuff_->GetDesc();

	//srvDesc.Format = resDesc.Format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = 1;

	//device->CreateShaderResourceView(texbuff_.Get(), //ビューと関連付けるバッファ
	//	&srvDesc, //テクスチャ設定情報
	//	cpuDescHandleSRV_
	//);

	//return true;
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial)
{
	//// 頂点バッファの設定
	//cmdList->IASetVertexBuffers(0, 1, &vbView_);
	//// インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView_);

	////マテリアル用定数バッファビューをセット
	//cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1_->GetGPUVirtualAddress());

	//// デスクリプタヒープの配列
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeap_.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//// シェーダリソースビューをセット
	//if (material.textureFilename.size() > 0)
	//{
	//	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV_);
	//}

	//// 描画コマンド
	//cmdList->DrawIndexedInstanced(((UINT)indices_.size()), 1, 0, 0, 0);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//全メッシュを描画
	for (auto& mesh : meshes)
	{
		mesh->Draw(cmdList);
	}
}

void Model::Initialize(const std::string& modelname)
{
	//objファイルを開く
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = "Resources/" + modelname + "/";

	//HRESULT result = S_FALSE;

	//ファイルストリーム
	std::ifstream file;

	file.open(directoryPath + filename);

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//モデル名を名前にコピー
	name = modelname;

	//メッシュを生成
	Mesh* mesh = new Mesh;
	int indexCountTex = 0;
	int indexCountNoTex = 0;

	std::vector<XMFLOAT3> positions;	//頂点座標
	std::vector<XMFLOAT3> normals;	//法線ベクトル
	std::vector<XMFLOAT2> texcoords;	//テクスチャUV

	//1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角「スペース区切りで行の先頭文字列を取得
		std::string Key;

		getline(line_stream, Key, ' ');

		//先頭文字列がmtllibならマテリアル
		if (Key == "mtllib")
		{
			//マテリアルファイルの各読み込み
			std::string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		//先頭の文字列がgならグループの開始
		if (Key == "g")
		{
			//カレントのメッシュの情報が揃っているなら
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0)
			{
				//コンテナに登録
				meshes.emplace_back(mesh);
				//次のメッシュ生成
				mesh = new Mesh;
				indexCountTex = 0;
			}

			//グループ名読み込み
			string groupName;
			line_stream >> groupName;

			//メッシュに名前セット
			mesh->SetName(groupName);
		}

		//先頭文字列がvなら頂点座標
		if (Key == "v")
		{
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//頂点座標にデータを追加
			positions.emplace_back(position);

		}

		//先頭文字がvtならテクスチャ
		if (Key == "vt")
		{
			//U, V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			//V方向反転
			texcoord.y = 1.0f - texcoord.y;

			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字がvnなら法線ベクトル
		if (Key == "vn")
		{
			//X, Y, Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がusemtlならマテリアルを割り当て
		if (Key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr)
			{
				//マテリアル名読み込み
				string materialName;
				line_stream >> materialName;

				//マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials.find(materialName);
				if (itr != materials.end())
				{
					mesh->SetMaterial(itr->second);
				}
			}
		}

		//先頭文字列がfならポリゴン(三角形)
		if (Key == "f")
		{
			int faceIndexCount = 0;

			//半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				//頂点番号
				index_stream >> indexPosition;
				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, std::ios_base::cur);

				//マテリアル、テクスチャがある場合
				if (material && material->textureFileName.size() > 0)
				{
					index_stream >> indexTexcoord;
					index_stream.seekg(1, std::ios_base::cur);	//スラッシュを飛ばす
					index_stream >> indexNormal;

					//頂点データの追加
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);

					/*頂点インデックスに追加
					indices_.emplace_back((unsigned short)indices_.size());*/
				}

				else
				{
					char c;
					index_stream >> c;
					//スラッシュ2連続の場合、頂点番号のみ
					if (c == '/')
					{
						//頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}

					else
					{
						index_stream.seekg(-1, ios_base::cur);	//1文字戻る
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
						index_stream >> indexNormal;
						//頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
				}
				//インデックスデータの追加
				if (faceIndexCount >= 3)
				{
					//四角形ポリゴン4点目なので
					//四角形の0,1,2,3のうちの2,3,0で三角形を構築
					mesh->AddIndex(indexCountTex - 1);
					mesh->AddIndex(indexCountTex);
					mesh->AddIndex(indexCountTex - 3);
				}

				else
				{
					mesh->AddIndex(indexCountTex);
				}
				indexCountTex++;
				faceIndexCount++;
			}
		}
	}
		//ファイルを閉じる
		file.close();

		//コンテナに登録
		meshes.emplace_back(mesh);

		//メッシュのマテリアルチェック
		for (auto& m : meshes)
		{
			//マテリアルの割り当てがない
			if (m->GetMaterial() == nullptr)
			{
				if (defaultMaterial == nullptr)
				{
					//デフォルトマテリアルを生成
					defaultMaterial = Material::Create();
					defaultMaterial->name = "no material";
					materials.emplace(defaultMaterial->name, defaultMaterial);
				}
				//デフォルトマテリアルをセット
				m->SetMaterial(defaultMaterial);
			}
		}

		//メッシュバッファの生成
		for (auto& m : meshes)
		{
			m->CreateBuffers();
		}

		//マテリアルの数値を定数バッファに反映
		for (auto& m : materials)
		{
			m.second->Update();
		}

		//デスクリプタヒープの生成
		InitializeDescriptorHeap();

		//テクスチャの読み込み
		LoadTexture();
	}

void Model::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	//マテリアルの数
	size_t count = materials.size();

	// デスクリプタヒープを生成	
	if (count > 0)
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
		descHeapDesc.NumDescriptors = (UINT)count; // シェーダーリソースビュー1つ
		result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
		if (FAILED(result)) {
			assert(0);
		}
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	Material* material = nullptr;

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字を取得
		std::string key;
		getline(line_stream, key, ' ');

		//先頭タブ文字はスルーする
		if (key[0] == '\t')
		{
			key.erase(key.begin());		//先頭の文字列を削除
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			////マテリアル名読み込み
			//line_stream >> material.name;
			//既にマテリアルがあれば
			if (material)
			{
				//マテリアルコンテナに登録
				AddMaterial(material);
			}

			//新しいマテリアルを生成
			material = Material::Create();
			//マテリアル名読み込み
			line_stream >> material->name;
		}

		//先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}

		//先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}

		//先頭文字列がKsならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}

		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			//テクスチャファイル名読み込み
			line_stream >> material->textureFileName;

			//テクスチャ読み込み
			//LoadTexture(directoryPath, material.textureFilename);

			//フルパスからファイル名を取り出す
			size_t pos1;
			pos1 = material->textureFileName.rfind('\\');
			if (pos1 != string::npos)
			{
				material->textureFileName = material->textureFileName.substr(pos1 + 1, material->textureFileName.size() - pos1 - 1);
			}

			pos1 = material->textureFileName.rfind('/');
			if (pos1 != string::npos)
			{
				material->textureFileName = material->textureFileName.substr(pos1 + 1, material->textureFileName.size() - pos1 - 1);
			}

		}
	}

	//ファイルを閉じる
	file.close();

	//マテリアルを登録
	if (material)
	{
		AddMaterial(material);
	}

}

void Model::AddMaterial(Material* material)
{
	//コンテナに登録
	materials.emplace(material->name, material);
}

void Model::CreateBuffers()
{
	//HRESULT result = S_FALSE;

	//UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * vertices_.size());
	//UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());


	//// 頂点バッファ生成
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff_));
	//if (FAILED(result)) {
	//	assert(0);
	//	return;
	//}

	//// インデックスバッファ生成
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&indexBuff__));
	//if (FAILED(result)) {
	//	assert(0);
	//	return;
	//}

	//// 頂点バッファへのデータ転送
	//VertexPosNormalUvSkin* vertMap = nullptr;
	//result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	//if (SUCCEEDED(result)) {
	//	std::copy(vertices_.begin(), vertices_.end(), vertMap);
	//	vertBuff_->Unmap(0, nullptr);
	//}

	//// インデックスバッファへのデータ転送
	//unsigned short* indexMap = nullptr;
	//result = indexBuff__->Map(0, nullptr, (void**)&indexMap);
	//if(SUCCEEDED(result))
	//{
	//	std::copy(indices_.begin(), indices_.end(), indexMap);
	//	indexBuff__->Unmap(0, nullptr);
	//}

	//// 頂点バッファビューの作成
	//vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//vbView_.SizeInBytes = sizeVB;
	//vbView_.StrideInBytes = sizeof(vertices_[0]);

	//// インデックスバッファビューの作成
	//ibView_.BufferLocation = indexBuff__->GetGPUVirtualAddress();
	//ibView_.Format = DXGI_FORMAT_R16_UINT;
	//ibView_.SizeInBytes = sizeIB;


	////マテリアル用定数バッファの生成
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffB1_));
	//assert(SUCCEEDED(result));

	////マテリアル用定数バッファへデータ転送B1
	//ConstBufferDataB1* constMap1 = nullptr;
	//result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
	//assert(SUCCEEDED(result));
	//constMap1->ambient = material.ambient;
	//constMap1->diffuse = material.diffuse;
	//constMap1->specular = material.diffuse;
	//constMap1->alpha = material.alpha;
	//constBuffB1_->Unmap(0, nullptr);
}
