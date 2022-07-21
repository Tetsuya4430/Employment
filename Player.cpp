#include "Player.h"
#include "FbxLoader.h"

void Player::Initialize()
{
	//モデルを指定してFBXファイルを読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
}

void Player::Update()
{
}

void Player::Draw()
{
}
