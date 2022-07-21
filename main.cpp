//クラス化したヘッダファイルのインクルード
#include "MyGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	FrameWork* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}