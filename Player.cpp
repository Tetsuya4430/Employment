#include "Player.h"
#include "FbxLoader.h"

void Player::Initialize()
{
	//���f�����w�肵��FBX�t�@�C����ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
}

void Player::Update()
{
}

void Player::Draw()
{
}
