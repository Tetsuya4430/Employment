#pragma once
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "Boss.h"
#include "BossBullet.h"
#include "BossParts.h"
#include "FbxLoader.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "Particle.h"
#include "DefenceObject.h"
#include "ImGuiManager.h"

#include <memory>

/// <summary>
/// フレームワーク
/// </summary>
class FrameWork
{
public:

	/// <summary>
	/// 実行
	/// </summary>
	void Run();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual  void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	virtual bool IsEndRequest() { return endRequest_; }

protected:
	bool endRequest_ = false;

	WinApp* winApp = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	//所有者
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
	ImGuiManager* ImGui = nullptr;
};

