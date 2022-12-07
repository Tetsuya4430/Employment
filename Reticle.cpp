#include "Reticle.h"
#include "Input.h"
#include "Bullet.h"
#include "Controller.h"
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#pragma comment(lib, "d3dcompiler.lib")


#include "DebugText.h"
#include <iomanip>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Reticle::device = nullptr;
ID3D12GraphicsCommandList* Reticle::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Reticle::rootsignature;
ComPtr<ID3D12PipelineState> Reticle::pipelinestate;

Reticle* Reticle::Create(Model* model, Camera* camera)
{
	//3Dオブジェクトのインスタンスを生成
	Reticle* instance = new Reticle();
	if (instance == nullptr)
	{
		return nullptr;
	}

	//初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	//モデルのセット
	if (model)
	{
		instance->SetModel(model);
	}

	//カメラのセット
	if (camera)
	{
		instance->SetCamera(camera);
	}

	return instance;
}

Reticle* Reticle::GetInstance()
{
	static Reticle instance;

	return &instance;
}

bool Reticle::Initialize()
{
	// nullptrチェック
	assert(device);

	//コントローラー初期化
	InitInput();

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0_));

	return true;
}

bool Reticle::Finalize()
{
	ReleaseInput();

	return true;
}

void Reticle::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Reticle::cmdList);

	//モデルの紐づけがない場合は描画しない
	if (model_ == nullptr)
	{
		return;
	}


	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());

	model_->Draw(cmdList, 1);
}

void Reticle::Update(XMFLOAT3 PlayerPos)
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	//マウス座標取得
	GetCursorPos(&MousePosition);
	ShowCursor(false);	//カーソルを表示しない

	//クライアントエリア座標に変換
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &MousePosition);

	//ビューポート行列
	XMMATRIX matViewport = XMMatrixIdentity();	//単位行列

	matViewport.r[0].m128_f32[0] = 640;
	matViewport.r[1].m128_f32[1] = -360;
	matViewport.r[3].m128_f32[0] = 640;
	matViewport.r[3].m128_f32[1] = 360;

	//ビュープロジェクションビューポート合成行列
	XMMATRIX matVPV = camera_->matView * camera_->matProjection * matViewport;

	//合成行列の逆行列をを計算する
	XMMATRIX matInverseVPV = XMMatrixInverse(nullptr, matVPV);

	//スクリーン座標
	XMVECTOR PosNear = { MousePosition.x, MousePosition.y, 0 };
	XMVECTOR PosFar = { MousePosition.x, MousePosition.y, 1 };

	//スクリーン座標系からワールド座標系へ
	PosNear = XMVector3Transform(PosNear, matInverseVPV);
	PosFar = XMVector3Transform(PosFar, matInverseVPV);

	//マウスレイの方向
	XMVECTOR mouseDirection = PosNear - PosFar;
	mouseDirection = XMVector3Normalize(mouseDirection);

	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 30.0f;

	//XMVECTOR VecPos;

	//VecPos = XMLoadFloat3(&position_);

	//XMLoadFloat3(&position_) = PosNear - mouseDirection + XMLoadFloat(&kDistanceTestObject);

	XMStoreFloat3(&position_, PosNear - mouseDirection + XMLoadFloat(&kDistanceTestObject));

	//position_.z = 20.0f;

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
	matWorld_ *= matTrans; // ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent_ != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}

	const XMMATRIX& matViewProjection = camera_->GetmatViewProjection();

	// 定数バッファへデータ転送B0
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0_->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	//constMap->mat = matWorld_ * matView * matProjection;	// 行列の合成
	constMap->mat = matWorld_ * matViewProjection;	// 行列の合成
	constBuffB0_->Unmap(0, nullptr);

	//更新処理

	

	//デバッグテキスト
	//マウス座標
	std::ostringstream Mousestr;
	Mousestr << "MousePosition("
		<< std::fixed << std::setprecision(2)
		<< MousePosition.x << ","
		<< MousePosition.y << ")";

	DebugText::GetInstance()->Print(Mousestr.str(), 0, 50, 2.0f);

	//ニア
	std::ostringstream Nearstr;
	Nearstr << "PosNear("
		<< std::fixed << std::setprecision(5)
		<< PosNear.m128_f32[0] << ","
		<< PosNear.m128_f32[1] << ","
		<< PosNear.m128_f32[2] << ")";

	DebugText::GetInstance()->Print(Nearstr.str(), 0, 200, 2.0f);

	//ファー
	std::ostringstream Farstr;
	Farstr << "PosFar("
		<< std::fixed << std::setprecision(5)
		<< PosFar.m128_f32[0] << ","
		<< PosFar.m128_f32[1] << ","
		<< PosFar.m128_f32[2] << ")";

	DebugText::GetInstance()->Print(Farstr.str(), 0, 250, 2.0f);

	//オブジェクトの座標
	std::ostringstream Objstr;
	Objstr << "ObjPos("
		<< std::fixed << std::setprecision(5)
		<< position_.x << ","
		<< position_.y << ","
		<< position_.z << ")";

	DebugText::GetInstance()->Print(Objstr.str(), 0, 300, 2.0f);
	



	//プレイヤーの移動
	if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_S) || Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_W))
	{
		
		
		/*if (Input::GetInstance()->PushKey(DIK_RIGHT))
		{
			if (position_.x <= 45)
			{
				position_.x += ReticleMove;
			}
		}

		
		if (Input::GetInstance()->PushKey(DIK_LEFT))
		{
			if (position_.x >= -45)
			{
				position_.x -= ReticleMove;
			}
		}

		

		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			if (position_.y <= 20)
			{
				position_.y += ReticleMove;
			}
		}

		if (Input::GetInstance()->PushKey(DIK_DOWN))
		{
			if (position_.y >= -25)
			{
				position_.y -= ReticleMove;
			}
		}*/


		

		


		/*if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
		{
			
			if (IsButtonPush(ButtonKind::RightButton))
			{
				position_.x += ReticleMove;
			}

			if (IsButtonPush(ButtonKind::LeftButton))
			{
				position_.x -= ReticleMove;
			}

			if (IsButtonPush(ButtonKind::UpButton))
			{
				position_.y += ReticleMove;
			}

			if (IsButtonPush(ButtonKind::DownButton))
			{
				position_.y -= ReticleMove;
			}
		}*/



		//コントローラーの押下情報更新
		UpdateInput();
		//}
	}

	


	////プレイヤーが画面買外に進出しようとするときは押し戻す
	////右側
	//if (position_.x >= 45)
	//{
	//	position_.x = 45;
	//}

	////左側
	//if (position_.x <= -45)
	//{
	//	position_.x = -45;
	//}

	////上側
	//if (position_.y >= 20)
	//{
	//	position_.y = 20;
	//}

	////下側
	//if (position_.y <= -20)
	//{
	//	position_.y = -20;
	//}
}


XMFLOAT3 Reticle::GetWorldPosition()
{
	//ワールド座標を入れる変数
	XMFLOAT3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = position_.x;
	worldPos.y = position_.y;
	worldPos.z = position_.z;

	return worldPos;
}

bool Reticle::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{// nullptrチェック
	assert(device);

	Reticle::device = device;
	Reticle::cmdList = cmdList;

	Model::SetDevice(device);

	// カメラ初期化
	Camera::InitializeCamera(window_width, window_height);

	// パイプライン初期化
	InitializeGraphicsPipeline();

	// テクスチャ読み込み
	//LoadTexture();


	return true;
}





bool Reticle::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJVS.hlsl",	// シェーダファイル名
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

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJPS.hlsl",	// シェーダファイル名
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
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

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
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

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
