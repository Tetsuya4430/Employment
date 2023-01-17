#include "Player.h"
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
ID3D12Device* Player::device = nullptr;
ID3D12GraphicsCommandList* Player::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Player::rootsignature;
ComPtr<ID3D12PipelineState> Player::pipelinestate;

Player* Player::Create(Model* model, Camera* camera)
{
    //3Dオブジェクトのインスタンスを生成
    Player* instance = new Player();
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

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

bool Player::Initialize()
{
	// nullptrチェック
	assert(device);

	//コントローラー初期化
	InitInput();

	//レティクル初期化
	ReticlePos.x = position_.x;
	ReticlePos.y = position_.y;
	ReticlePos.z = position_.z + 30.0f;

	//プレイヤー各種ステータス初期化
	position_.z = -50;
	position_.y = -10;
	Level = 1;
	EXP = 0;

	audio->Initialize();


	Audio::GetInstance()->LoadWave("Avoid.wav");
	Audio::GetInstance()->LoadWave("LevelUp.wav");

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

bool Player::Finalize()
{
	ReleaseInput();

	return true;
}

void Player::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Player::cmdList);

	//モデルの紐づけがない場合は描画しない
	if (model_ == nullptr)
	{
		return;
	}


	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());

	model_->Draw(cmdList, 1);
}

void Player::Update()
{
	/*if (position_.z < 0)
	{
		position_.z += 3;
	}*/

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

	Mag = 0.0f;

	if (Level == 1)
	{
		Speed = 0.5f;
	}

	else if (Level == 2)
	{
		Speed = 0.75f;
	}

	else if (Level == 3)
	{
		Speed = 1.0f;
	}

	

	//コントローラーの押下情報更新
	UpdateInput();

	//プレイヤーの移動
	//キーボード
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S))
	{
		
			K = 0.5f;
		
			if (MoveCanFlag == true)
			{


				if (Input::GetInstance()->PushKey(DIK_A) && Input::GetInstance()->PushKey(DIK_W))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_A) && Input::GetInstance()->PushKey(DIK_S))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_D) && Input::GetInstance()->PushKey(DIK_W))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_D) && Input::GetInstance()->PushKey(DIK_S))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else
				{
					Mag = 1.0f;

					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}


				MoveFlag = 1;
				if (Input::GetInstance()->PushKey(DIK_D))
				{
					moveR = true;
					if (position_.x <= 45)
					{
						position_.x += Speed * Mag;
						Old.x = Speed * Mag;
					}
				}


				if (Input::GetInstance()->PushKey(DIK_A))
				{
					moveL = true;
					if (position_.x >= -45)
					{
						position_.x -= Speed * Mag;
						Old.x = -Speed * Mag;
					}
				}


				if (Input::GetInstance()->PushKey(DIK_W))
				{
					moveU = true;
					if (position_.y <= 20)
					{
						position_.y += Speed * Mag;
						Old.y = Speed * Mag;
					}
				}

				if (Input::GetInstance()->PushKey(DIK_S))
				{
					moveD = true;
					if (position_.y >= -25)
					{
						position_.y -= Speed * Mag;
						Old.y = -Speed * Mag;
					}
				}

				else
				{
					MoveFlag = 0;
				}
			}
			
			//プレイヤーが移動していないときはプレイヤーの回転角度を徐々に戻す
			if (MoveFlag == 0)
			{
				if (Avoidanceflag_X == false && Avoidanceflag_Y == false)
				{
					/*if (rotation_.z > 0)
					{
						rotation_.z -= 0.5f;
					}

					if (rotation_.z < 0)
					{
						rotation_.z += 0.5f;
					}*/
				}
			}


			if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			{
				MoveFlag = 1;
				if (IsButtonPush(ButtonKind::RightButton))
				{
					position_.x += 0.5f;
					if (RotlimR < rotation_.z)
					{
						rotation_.z -= 0.3f;
					}
				}

				if (IsButtonPush(ButtonKind::LeftButton))
				{
					MoveFlag = 1;
					position_.x -= 0.5f;
					if (RotlimL > rotation_.z)
					{
						rotation_.z += 0.3f;
					}
				}

				if (IsButtonPush(ButtonKind::UpButton))
				{
					position_.y += 0.5f;
				}

				if (IsButtonPush(ButtonKind::DownButton))
				{
					position_.y -= 0.5f;
				}
			}

			else
			{
				MoveFlag = 0;
			}


		//}
	}


	//キーボード
	if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
	{

		K = 0.5f;

		if (MoveCanFlag == true)
		{


			if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::UpButton))
			{
				Mag = 0.71f;
				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}

			else if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::DownButton))
			{
				Mag = 0.71f;
				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}

			else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::UpButton))
			{
				Mag = 0.71f;
				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}

			else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::DownButton))
			{
				Mag = 0.71f;
				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}

			else
			{
				Mag = 1.0f;

				moveR = false;
				moveL = false;
				moveU = false;
				moveD = false;
			}


			MoveFlag = 1;
			if (IsButtonPush(ButtonKind::RightButton))
			{
				moveR = true;
				if (position_.x <= 45)
				{
					position_.x += Speed * Mag;
					Old.x = Speed * Mag;
				}
			}


			if (IsButtonPush(ButtonKind::LeftButton))
			{
				moveL = true;
				if (position_.x >= -45)
				{
					position_.x -= Speed * Mag;
					Old.x = -Speed * Mag;
				}
			}


			if (IsButtonPush(ButtonKind::UpButton))
			{
				moveU = true;
				if (position_.y <= 20)
				{
					position_.y += Speed * Mag;
					Old.y = Speed * Mag;
				}
			}

			if (IsButtonPush(ButtonKind::DownButton))
			{
				moveD = true;
				if (position_.y >= -25)
				{
					position_.y -= Speed * Mag;
					Old.y = -Speed * Mag;
				}
			}

			else
			{
				MoveFlag = 0;
			}
		}


		//プレイヤーが移動していないときはプレイヤーの回転角度を徐々に戻す
		if (MoveFlag == 0)
		{
			if (Avoidanceflag_X == false && Avoidanceflag_Y == false)
			{
				/*if (rotation_.z > 0)
				{
					rotation_.z -= 0.5f;
				}

				if (rotation_.z < 0)
				{
					rotation_.z += 0.5f;
				}*/
			}
		}


		if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
		{
			MoveFlag = 1;
			if (IsButtonPush(ButtonKind::RightButton))
			{
				position_.x += 0.5f;
				if (RotlimR < rotation_.z)
				{
					rotation_.z -= 0.3f;
				}
			}

			if (IsButtonPush(ButtonKind::LeftButton))
			{
				MoveFlag = 1;
				position_.x -= 0.5f;
				if (RotlimL > rotation_.z)
				{
					rotation_.z += 0.3f;
				}
			}

			if (IsButtonPush(ButtonKind::UpButton))
			{
				position_.y += 0.5f;
			}

			if (IsButtonPush(ButtonKind::DownButton))
			{
				position_.y -= 0.5f;
			}
		}

		else
		{
			MoveFlag = 0;
		}


		//コントローラーの押下情報更新
		UpdateInput();
		//}
	}

	//もし移動していないのであれば
	if (Mag == 0.0f)
	{
		//移動量分動かす
		if (K > 0)
		{
			if (moveR == true)
			{
				K -= 0.01f;
				position_.x += K;
			}

			if (moveL == true)
			{
				K -= 0.01f;
				position_.x -= K;
			}

			if (moveU == true)
			{
				K -= 0.01f;
				position_.y += K;
			}

			if (moveD == true)
			{
				K -= 0.01f;
				position_.y -= K;
			}

			//斜め移動
			else if (moveR == true && moveU == true)
			{
				K -= 0.01f;
				position_.x += K;
				position_.y += K;
			}

			else if (moveR == true && moveD == true)
			{
				K -= 0.01f;
				position_.x += K;
				position_.y -= K;
			}

			else if (moveL == true && moveD == true)
			{
				K -= 0.01f;
				position_.x -= K;
				position_.y -= K;
			}

			else if (moveL == true && moveU == true)
			{
				K -= 0.01f;
				position_.x -= K;
				position_.y += K;
			}
		}

	}

//デバッグ用
	//オブジェクトの座標
	//std::ostringstream Objstr;
	//Objstr << "ObjPos("
	//	<< std::fixed << std::setprecision(5)
	//	<< position_.x << ","
	//	<< position_.y << ","
	//	<< position_.z << ")";

	//DebugText::GetInstance()->Print(Objstr.str(), 0, 300, 2.0f);

	////mag
	//std::ostringstream Magstr;
	//Magstr << "Mag("
	//	<< std::fixed << std::setprecision(5)
	//	<< Mag << ")";

	//DebugText::GetInstance()->Print(Magstr.str(), 0, 0, 2.0f);

	////Old
	//std::ostringstream Oldstr;
	//Oldstr << "OldPos("
	//	<< std::fixed << std::setprecision(5)
	//	<< Old.x << ","
	//	<< Old.y << ")";

	//DebugText::GetInstance()->Print(Oldstr.str(), 0, 50, 2.0f);


	////もし移動が終了しているなら
	//if (Mag == 0.0)
	//{
	//	//まだ移動量が0でないのならば
	//	if (Old.x != 0.0f)
	//	{
	//		if (Old.x < 0.0f)
	//		{
	//			Old.x += 0.1f;
	//		}

	//		if (Old.x > 0.0f)
	//		{
	//			Old.x -= 0.1f;
	//		}
	//	}

	//	//まだ移動量が0でないのならば
	//	if (Old.y != 0.0f)
	//	{
	//		if (Old.y < 0.0f)
	//		{
	//			Old.y += 0.1f;
	//		}

	//		if (Old.y > 0.0f)
	//		{
	//			Old.y -= 0.1f;
	//		}
	//	}

	//	//移動領分動かす
	//	position_.x += Old.x;
	//	position_.y += Old.y;
	//}

	//回避モーション
	//キーボード
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S))
	{

		if (AvoidanceTimer_X == 0)
		{
			//右回避
			if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_D) && Avoidanceflag_X == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.x + AvoidDistance_X;
				RolePos = rotation_.z - RoleDistance;
				RotFlag_R = true;

				Avoidanceflag_X = true;
			}

			//左回避
			if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_A) && Avoidanceflag_X == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.x - AvoidDistance_X;
				RolePos = rotation_.z + RoleDistance;
				RotFlag_L = true;

				Avoidanceflag_X = true;
			}
		}

		if (AvoidanceTimer_Y == 0)
		{
			//上回避
			if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_W) && Avoidanceflag_Y == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.y + AvoidDistance_Y;

				Avoidanceflag_Y = true;
			}

			//下回避
			if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_S) && Avoidanceflag_Y == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.y - AvoidDistance_Y;

				Avoidanceflag_Y = true;
			}
		}
	}

	//斜め回避は受け付けない
	if (Input::GetInstance()->PushKey(DIK_W) && Input::GetInstance()->PushKey(DIK_D))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (Input::GetInstance()->PushKey(DIK_W) && Input::GetInstance()->PushKey(DIK_A))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (Input::GetInstance()->PushKey(DIK_S) && Input::GetInstance()->PushKey(DIK_D))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (Input::GetInstance()->PushKey(DIK_S) && Input::GetInstance()->PushKey(DIK_A))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	//コントローラー
	if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
	{

		if (AvoidanceTimer_X == 0)
		{
			//右回避
			if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::RightButton) && Avoidanceflag_X == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.x + AvoidDistance_X;
				RolePos = rotation_.z - RoleDistance;
				RotFlag_R = true;

				Avoidanceflag_X = true;
			}

			//左回避
			if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::LeftButton) && Avoidanceflag_X == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.x - AvoidDistance_X;
				RolePos = rotation_.z + RoleDistance;
				RotFlag_L = true;

				Avoidanceflag_X = true;
			}
		}

		if (AvoidanceTimer_Y == 0)
		{
			//上回避
			if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::UpButton) && Avoidanceflag_Y == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.y + AvoidDistance_Y;

				Avoidanceflag_Y = true;
			}

			//下回避
			if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::DownButton) && Avoidanceflag_Y == false)
			{
				Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
				PointPos = position_.y - AvoidDistance_Y;

				Avoidanceflag_Y = true;
			}
		}
	}

	//斜め回避は受け付けない
	if (IsButtonPush(ButtonKind::UpButton) && IsButtonPush(ButtonKind::RightButton))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (IsButtonPush(ButtonKind::UpButton) && IsButtonPush(ButtonKind::LeftButton))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (IsButtonPush(ButtonKind::DownButton) && IsButtonPush(ButtonKind::RightButton))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	if (IsButtonPush(ButtonKind::DownButton) && IsButtonPush(ButtonKind::LeftButton))
	{
		Avoidanceflag_X = false;
		Avoidanceflag_Y = false;
	}

	//実際の回避処理
	if (Avoidanceflag_X == true)
	{
		AvoidanceTimer_X += 1;

		
		dx = (PointPos - position_.x) / AvoidCount;
		position_.x += dx;

		if (RotFlag_R == true)
		{
			rx = (RolePos - rotation_.z) / AvoidCount;
			rotation_.z += rx;
		}

		else if (RotFlag_L == true)
		{
			rx = (RolePos - rotation_.z) / AvoidCount;
			rotation_.z += rx;
		}

		if (AvoidanceTimer_X >= 20)
		{
			AvoidanceTimer_X = 0;
			rotation_.z = 0;
			RotFlag_R = false;
			RotFlag_L = false;
			Avoidanceflag_X = false;
		}

	}

	if (Avoidanceflag_Y == true)
	{
		AvoidanceTimer_Y += 1;

		
		dx = (PointPos - position_.y) / AvoidCount;
		position_.y += dx;

		if (AvoidanceTimer_Y >= 20)
		{
			AvoidanceTimer_Y = 0;
			Avoidanceflag_Y = false;
		}
	}


	//プレイヤーが画面買外に進出しようとするときは押し戻す
	//右側
	if (position_.x >= 45)
	{
		position_.x = 45;
	}

	//左側
	if (position_.x <= -45)
	{
		position_.x = -45;
	}

	//上側
	if (position_.y >= 20)
	{
		position_.y = 20;
	}

	//下側
	if (position_.y <= -20)
	{
		position_.y = -20;
	}

	//レベルの管理
	//経験値が5たまったら
	if (EXP == 5 && Level < 3)
	{
		Audio::GetInstance()->PlayWave("LevelUp.wav", 0.3f, false);
		//経験値をリセットして、レベルを1上昇
		EXP = 0;
		Level += 1;
	}
}


XMFLOAT3 Player::GetWorldPosition()
{
	//ワールド座標を入れる変数
	XMFLOAT3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = position_.x;
	worldPos.y = position_.y;
	worldPos.z = position_.z;

	return worldPos;
}

bool Player::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{// nullptrチェック
    assert(device);

	Player::device = device;
	Player::cmdList = cmdList;

    Model::SetDevice(device);

    // カメラ初期化
    Camera::InitializeCamera(window_width, window_height);

    // パイプライン初期化
    InitializeGraphicsPipeline();

    // テクスチャ読み込み
    //LoadTexture();


    return true;
}



bool Player::InitializeGraphicsPipeline()
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
