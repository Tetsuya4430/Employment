#pragma once

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class GameClear : public BaseScene
{
public:
	//������
	void Initialize(/*DirectXCommon* dxCommon*/) override;

	//�I������
	void Finalize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;


private:
	DirectXCommon* dxCommon = nullptr;
	Sprite* sprite = nullptr;


	Sprite* LoadBG = nullptr;


	Audio* audio = Audio::GetInstance();

	//���[�h�t���O
	bool LoadFlag = false;
};

