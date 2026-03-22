#include<string>
#include<windows.h>
#include"DxLib.h"
#include "Menu.h"
using namespace std;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	ChangeWindowMode(TRUE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetGraphMode(640 * 2, 480 * 2, 32);
	SetWindowSize(640 * 2, 480 * 2);

	//Game game;
	//game.MainGame();
	Menu menu;
	menu.MenuMain();


	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;
}