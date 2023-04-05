#include "Particle.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

//静的メンバ変数の実体
const float Particle::radius = 5.0f;
const float Particle::prizmHeight = 8.0f;
ID3D12Device* Particle::device = nullptr;
UINT Particle::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* Particle::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Particle::rootsignature;
ComPtr<ID3D12PipelineState> Particle::pipelinestate;
ComPtr<ID3D12DescriptorHeap> Particle::descHeap;
ComPtr<ID3D12Resource> Particle::vertBuff;
ComPtr<ID3D12Resource> Particle::texBuff[MaxTexture];
CD3DX12_CPU_DESCRIPTOR_HANDLE Particle::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE Particle::gpuDescHandleSRV;
XMMATRIX Particle::matView{};
XMMATRIX matProjection{};
XMFLOAT3 Particle::eye = { 0, 0, -50.0f };
XMFLOAT3 Particle::target = { 0, 0, 0 };
XMFLOAT3 Particle::up = { 0, 1, 0};
D3D12_VERTEX_BUFFER_VIEW Particle::vbView{};
Particle::VertexPos Particle::vertices[vertexCount];

//XMFLOAT3同士の加算処理
const DirectX::XMFLOAT3 operator+(const::DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;

	return result;
}

Particle* Particle::GetInstance()
{
	static Particle instance;

	return &instance;
}

bool Particle::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{
	// nullptrチェック
	assert(device);

	Particle::device = device;
	Particle::cmdList = cmdList;

	//デスクリプタヒープの初期化
	InitializeDescriptorHeap();

	//カメラ初期化
	Camera::InitializeCamera(window_width, window_height);

	//パイプライン初期化
	InitializeGraphicsPipeline();

	//モデル生成
	CreateModel();


	return true;
}

void Particle::PreDraw()
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Particle::PostDraw()
{
	//コマンドリストを解除
	//Particle::cmdList = nullptr;
}

Particle* Particle::Create(UINT texnumber, Camera* camera)
{
	// 3Dオブジェクトのインスタンスを生成
	Particle* particle = new Particle();
	if (particle == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!particle->Initialize(texnumber)) {
		delete particle;
		assert(0);
		return nullptr;
	}

	if (camera)
	{
		particle->SetCamera(camera);
	}

	return particle;
}

void Particle::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texnumber)
{
	cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texnumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
}

bool Particle::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	//デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = MaxTexture;	//シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	//デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool Particle::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob; //ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{//スケール
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{//色
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{//回転
			"ROTATION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//デプスの書き込み禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	//半透明合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	//減算合成
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return result;
	}

	return true;
}

void Particle::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	HRESULT result;

	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0);	//生データ抽出

	//リソース設定
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	//テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,	//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texBuff[texnumber] ));

	

	//テクスチャバッファに転送
	result = texBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,		//1ラインサイズ
		(UINT)img->slicePitch	//全サイズ
	);


	//シェーダーリソースビュー設定
	//cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	//gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	//D3D12_RESOURCE_DESC resDesc = texBuff[texnumber]->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//ヒープのtexnumberにシェーダーリソースビューを作成
	device->CreateShaderResourceView(texBuff[texnumber].Get(),		//ビューと関連付けるバッファ
		&srvDesc,	//テクスチャ情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);

}

void Particle::CreateModel()
{
	HRESULT result = S_FALSE;

	std::vector<VertexPos> realVertices;
	
	////四角形の頂点データ
	//VertexPos verticesPoint[] = {
	//	{{0.0f, 0.0f, 0.0f}}
	//};

	////メンバ変数にコピー
	//std::copy(std::begin(verticesPoint), std::end(verticesPoint), vertices);

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	//頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}

bool Particle::Initialize(UINT texnumber)
{
	// nullptrチェック
	assert(device);

	//テクスチャ番号を書き込む
	this->texNumber = texnumber;

	//指定番号の画像が読み込み済みなら
	if (texBuff[texNumber])
	{
		//テクスチャ情報を取得
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
	}

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	//テクスチャ読み込み
	//LoadTexture(texnumber, filename);

	return true;
}

void Particle::Update(/*XMFLOAT4 color*/)
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ワールド行列の合成
	matWorld_ = XMMatrixIdentity(); // 変形をリセット
	matWorld_ *= matScale; // ワールド行列にスケーリングを反映
	matWorld_ *= matRot; // ワールド行列に回転を反映
	//matWorld_ *= camera_->GetBillBoardY();
	matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	//寿命が尽きたパーティクルを全削除
	particles.remove_if([](Particle_& x)
		{
			return x.frame >= x.num_frame;
		}
	);

	//全パーティクル更新
	for (std::forward_list<Particle_>::iterator it = particles.begin();
		it != particles.end(); it++)
	{
		//経過フレーム数をカウント
		it->frame++;
		//進行度を0～1の範囲に換算
		float f = (float)it->num_frame / it->frame;
		//速度に加速度を加算
		it ->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;
		//スケールの線形補間
		it->scale = (it->scale_end - it->scale_start) / f;
		it->scale += it->scale_start;
		//色の値を渡す
		it->color = it->color;
		//回転の値を渡す
		it->rotation = rotation_;
	}

	//頂点バッファへデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	if (SUCCEEDED(result))
	{
		//パーティクルの情報を1つずつ反映
		for (std::forward_list<Particle_>::iterator it = particles.begin();
			it != particles.end(); it++)
		{
			//座標
			vertMap->pos = it->position;
			//スケール
			vertMap->scale = it->scale;
			//色
			vertMap->color = it->color;
			//回転
			vertMap->rotation = it->rotation;
			//次の頂点へ
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}

	const XMMATRIX& matViewProjection = camera_->GetmatViewProjection();

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color_;
	constMap->mat = matWorld_ * matViewProjection;	// 行列の合成
	constMap->matBillboard = camera_->matBillboard;
	constBuff->Unmap(0, nullptr);
}

void Particle::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Particle::cmdList);

	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	////インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView);

	//デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//シェーダーリソースビューをセット
	SetGraphicsRootDescriptorTable(1, texNumber/*gpuDescHandleSRV*/);
	//描画コマンド
	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

void Particle::AddParticle(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale, XMFLOAT4 color)
{
	//リストに要素を追加
	particles.emplace_front();
	//追加した要素の参照
	Particle_& p = particles.front();
	//値のセット
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	//p.scale = start_scale;
	p.scale_start = start_scale;
	p.scale_end = end_scale;
	p.color = color;
}

void Particle::CreateParticleInfo(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布(座標)
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布(速度)
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//追加
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::DeathParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布(座標)
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布(速度)
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//追加
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::FireParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT3 Speed, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//引数の値を貰う
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x/* +(float)rand() / RAND_MAX * md_width - md_width/ 2.0f*/;
		pos.y = Position.y/* + (float)rand() / RAND_MAX * md_width - md_width / 2.0f*/;
		pos.z = Position.z + 10;
		//パーティクルの速度
		const float md_vel = 0.5;
		XMFLOAT3 vel{};
		vel.x = Speed.x;
		vel.y = Speed.y;
		vel.z -= Speed.z / 10;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		//const float md_acc = 0.001f;
		//acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//追加
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::EnemyFireParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT3 Speed, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//引数の値を貰う
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//パーティクルの速度
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.x += Speed.x / 10;
		vel.y += Speed.y / 10;
		vel.z -= Speed.z / 10;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//追加
		AddParticle(ParticleLife, pos, vel, rot, StartScale, EndScale, color);

	}
}

void Particle::PlayerLevelUpParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//中心座標
		XMFLOAT3 Center{};
		//半径
		float Radius;
		//角度
		float angle;
		//半径の長さ
		float Length;

		//引数の値を貰う
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//パーティクルの速度
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = 0.5f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//追加
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::LevelUpParticle(int PartNum, XMFLOAT3 Position, XMFLOAT3 EndPosition, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//中心座標
		XMFLOAT3 Center{};
		//半径
		float Radius;
		//角度
		float angle;
		//半径の長さ
		float Length;

		//引数の値を貰う
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z;
		//パーティクルの速度
		const float md_vel = Vel;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = 0.5f;
		vel.z = 0;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//追加
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::JettParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT3 Speed, XMFLOAT4 color)
{
	for (int i = 0; i < PartNum; i++)
	{
		//引数の値を貰う
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x/* +(float)rand() / RAND_MAX * md_width - md_width/ 2.0f*/;
		pos.y = Position.y/* + (float)rand() / RAND_MAX * md_width - md_width / 2.0f*/;
		pos.z = Position.z + 10;
		//パーティクルの速度
		const float md_vel = 0.2;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = Speed.y;
		vel.z -= Speed.z / 10;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		//const float md_acc = 0.001f;
		//acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		//追加
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

void Particle::WarningParticle(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale, XMFLOAT3 Speed, XMFLOAT4 color)
{
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	for (int i = 0; i < PartNum; i++)
	{
		//引数の値を貰う
		const float md_width = 10.0f;
		XMFLOAT3 pos{};
		pos.x = Position.x;
		pos.y = Position.y;
		pos.z = Position.z + 10;
		//パーティクルの速度
		const float md_vel = 0.5;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.x += Speed.x / 10;;
		vel.y += Speed.y / 10;
		vel.z -= Speed.z / 10;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		//const float md_acc = 0.001f;
		//acc.y = -(float)rand() / RAND_MAX * md_acc;

		XMFLOAT3 rot{};

		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;

		//追加
		AddParticle(ParticleLife, pos, vel, acc, StartScale, EndScale, color);

	}
}

