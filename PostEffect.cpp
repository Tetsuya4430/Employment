#include "PostEffect.h"
#include"WinApp.h"
#include <d3dx12.h>

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12RootSignature> PostEffect::rootSignature;
ComPtr<ID3D12PipelineState> PostEffect::pipelineState;

PostEffect::PostEffect(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
	:Sprite(
		100,	//テクスチャ番号
		{0, 0},	//アンカーポイント
		false,	//左右反転フラグ
		false	//上下反転フラグ
	)
{
}

void PostEffect::Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;

	//基底クラスとしての初期化
	//Sprite;; Initialize(texNumber, anchorpoint, isFlipX, isFlipY);


	//メンバ変数に書き込む
	texNumber_ = texNumber;
	anchorpoint_ = anchorpoint;
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//頂点データ
	VertexPosUv vertices[] = {
		{{0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},//左下
		{{0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},//左上
		{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},//右下
		{{100.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},//右上
	};

	//指定番号の画像が読み込み済みなら
	//if (spriteCommon->texBuff_[texNumber_])
	if (spriteCommon->GetTexBuff(texNumber))
	{
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(texNumber)->GetDesc();

		//スプライトの大きさを画像の解像度に合わせる
		size_ = { (float)resDesc.Width, (float)resDesc.Height };
		texSize_ = { (float)resDesc.Width, (float)resDesc.Height };
	}

	//頂点バッファ生成
	result = spriteCommon->GetDevice_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

	//---頂点バッファへのデータ転送---//
	//VertexPosUv* vertMap = nullptr;
	//result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	//memcpy(vertMap, vertices, sizeof(vertices));
	//vertBuff_->Unmap(0, nullptr);

	TransferVertexBuffer();

	//頂点バッファビューの生成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices);
	vbView_.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの生成
	result = spriteCommon->GetDevice_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_)
	);

	//定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);		//色指定(RGBA)
	constMap->mat = spriteCommon->GetmatProjection();
	constBuff_->Unmap(0, nullptr);

	//テクスチャリソースを設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

	//テクスチャバッファの生成
	result = spriteCommon->GetDevice_()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff));
		assert(SUCCEEDED(result));
		{
			//テクスチャクリア
			//画素数(1280x780 = 921600ピクセル)
			const UINT pixelCount = WinApp::window_width * WinApp::window_height;

			//画像1行分のデータサイズ
			const UINT rowPitch = sizeof(UINT) * WinApp::window_width;

			//画像全体のデータサイズ
			const UINT depthPitch = rowPitch * WinApp::window_height;

			//画像イメージ
			UINT* img = new UINT[pixelCount];
			for (int i = 0; i < pixelCount; i++)
			{
				img[i] = 0xff0000ff;
			}

			//テクスチャバッファにデータを転送
			result = texBuff->WriteToSubresource(0, nullptr,
				img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[]img;
		}

		//SRV用デスクリプタヒープ設定
		D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
		srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvDescHeapDesc.NumDescriptors = 1;

		//SRV用デスクリプタヒープ生成
		result = spriteCommon->GetDevice_()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
		assert(SUCCEEDED(result));

		//SRV設定
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		//デスクリプタヒープにSRV作成
		spriteCommon->GetDevice_()->CreateShaderResourceView(texBuff.Get(),
			&srvDesc,
			descHeapSRV->GetCPUDescriptorHandleForHeapStart()
			);
}

PostEffect* PostEffect::Create(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	//メモリ確保
	PostEffect* instance = new PostEffect(texNumber, anchorpoint, isFlipX, isFlipY);
	//インスタンス初期化
	instance->Initialize(texNumber, anchorpoint, isFlipX, isFlipY);

	return instance;
}

void PostEffect::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//非表示フラグがtrueなら
	if (isInvisible_)
	{
		//描画せずに抜ける
		return;
	}

	ID3D12GraphicsCommandList* commandList = spriteCommon->GetCommandList();

	////パイプラインステートの設定
	//commandList->SetPipelineState(pipelineState.Get());

	////ルートシグネチャ設定
	//commandList->SetGraphicsRootSignature(rootSignature.Get());

	////プリミティブ形状を設定
	//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//頂点バッファのセット
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	//ルートパラメータ0番に定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	//ルートパラメータ1番にシェーダーリソースビューをセット
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber_);

	//テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	commandList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//ポリゴンの描画
	commandList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDraw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = spriteCommon->GetCommandList();

	////パイプラインステートの設定
	//commandList->SetPipelineState(pipelineSet_.pipelinestate.Get());

	////ルートシグネチャの設定
	//commandList->SetGraphicsRootSignature(pipelineSet_.rootsignature.Get());

	////プリミティブ形状を設定
	//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	////テクスチャ用デスクリプタヒープの設定
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//commandList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());


}
