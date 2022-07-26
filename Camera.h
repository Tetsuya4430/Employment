#pragma once
#include <DirectXMath.h>

#include "Input.h"

class Camera
{
private: // エイリアス 
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:	//メンバ関数
	
	//インスタンス
	static Camera* GetInstance();

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveVector(XMFLOAT3 move);

	static void CameraMoveEyeVector(XMFLOAT3 move);

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

	//ビュー射影行列の取得
	static const XMMATRIX& GetmatViewProjection() { return matViewProjection; }

	//ビルボードの取得
	static const XMMATRIX& GetBillBoard() { return matBillboard; }

	static const XMMATRIX& GetBillBoardY() { return matBillboardY; }

	static void Update();


private:	//メンバ変数
	//ビュー射影行列
	static XMMATRIX matViewProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;




	//インプット
	Input* input = nullptr;

	XMFLOAT3 moveVec = { 2.0f, 0.0f, 0.0f };

protected:
	//ビルボード行列
	static XMMATRIX matBillboard;

	//Y軸回りビルボード行列
	static XMMATRIX matBillboardY;

public:
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
};
