#include "Menu.h"

Menu::Menu() {
	state = Scene::Title;
	menuselect = 0;
	maxstage = 0;
}

void Menu::ShowMenu() {
	ClearDrawScreen();
	DrawGraph(-20, -20, style.GetTitleBGStyle(), TRUE);
	DrawGraph(250, 80, style.GetTitleStyle(0), TRUE);
	DrawGraph(300, 255, style.GetTitleStyle(1), TRUE);
	DrawGraph(305, 305, style.GetTitleStyle(2), TRUE);
	DrawGraph(247, 266 + 48 * menuselect, style.GetArrowStyle(), TRUE);
	DrawGraph(200, 360, style.GetCommandStyle(0), TRUE);
	DrawGraph(290, 500, style.GetCommandStyle(3), TRUE);
}

void Menu::ShowGameOver() {
	for (int i = -2; i < 5; i++) {
		for (int j = -2; j < 20; j++) {
			DrawGraph(78+10*j, 268 + 10*i, style.GetVoidStyle(), TRUE);
		}
	}
	DrawGraph(100, 250, style.GetRestartStyle(), TRUE);
	DrawGraph(69, 256 + menuselect * 30, style.GetArrowStyle(), TRUE);
}

void Menu::ShowRank(Game& game) {
	if (game.GetScore() > 0) {
		if (game.GetScore() == record.GetScore(0))
		{
			DrawGraph(100, 500, style.GetStringStyle(10), TRUE);
		}
	}
}

bool Menu::MenuInput() {
	prevcommand = nowcommand;
	nowcommand = input.GetMenuCommand();
	if (!prevcommand.up && nowcommand.up) menuselect = (menuselect + 2) % 3;
	if (!prevcommand.down && nowcommand.down) menuselect = (menuselect + 1) % 3;
	if (!prevcommand.select && nowcommand.select) {
		if (menuselect == 0)
			state = Scene::Playing;
		else if(menuselect == 2){
			state = Scene::Challenge;
			menuselect = 0;
		}
		else{
			state = Scene::Playing2;
			menuselect = 0;
		}
	}
	if ((!prevcommand.up && nowcommand.up) || (!prevcommand.down && nowcommand.down))
		return true;
	else return false;
}

bool Menu::GameOverInput() {
	prevcommand = nowcommand;
	nowcommand = input.GetMenuCommand();
	if (!prevcommand.up && nowcommand.up) menuselect = (menuselect + 1) % 2;
	if (!prevcommand.down && nowcommand.down) menuselect = (menuselect + 1) % 2;
	if (!prevcommand.select && nowcommand.select) {
		if (menuselect == 0)
		{
			state = Scene::Playing;
		}
		if (menuselect == 1) {
			state = Scene::Title;
			menuselect = 0;
			ShowMenu();
		}
	}
	if ((!prevcommand.up && nowcommand.up) || (!prevcommand.down && nowcommand.down))
		return true;
	else return false;
}

bool Menu::GameOver2Input() {
	prevcommand = nowcommand;
	nowcommand = input.GetMenuCommand();
	if (!prevcommand.up && nowcommand.up) menuselect = (menuselect + 1) % 2;
	if (!prevcommand.down && nowcommand.down) menuselect = (menuselect + 1) % 2;
	if (!prevcommand.select && nowcommand.select) {
		if (menuselect == 0)
		{
			state = Scene::Playing2;
		}
		if (menuselect == 1) {
			state = Scene::Title;
			menuselect = 0;
			ShowMenu();
		}
	}
	if ((!prevcommand.up && nowcommand.up) || (!prevcommand.down && nowcommand.down))
		return true;
	else return false;
}

bool Menu::CGameOverInput() {
	prevcommand = nowcommand;
	nowcommand = input.GetMenuCommand();
	if (!prevcommand.up && nowcommand.up) menuselect = (menuselect + 1) % 2;
	if (!prevcommand.down && nowcommand.down) menuselect = (menuselect + 1) % 2;
	if (!prevcommand.select && nowcommand.select) {
		if (menuselect == 0)
		{
			state = Scene::Challenge;
		}
		if (menuselect == 1) {
			state = Scene::Title;
			menuselect = 0;
			ShowMenu();
		}
	}
	if ((!prevcommand.up && nowcommand.up) || (!prevcommand.down && nowcommand.down))
		return true;
	else return false;
}

void Menu::MenuMain() {
	ShowMenu();
	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//×ボタンで強制終了
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break; //Escで終了
			switch (state)
			{
			case Scene::Title: {
				if (MenuInput())
					ShowMenu();
				break;
			}
			case Scene::Playing: {
				Game* game;
				game = new Game(6);
				game->MainGame(record);
				ShowRank(*game);
				state = Scene::GameOver;
				ShowGameOver();
				delete game;
				break;
			}
			case Scene::GameOver: {
				if(GameOverInput())
					ShowGameOver();;
				break;
			}
			case Scene::Challenge: {
				Challenge* game;
				game = new Challenge(maxstage);
				game->MainChallenge(record);
				maxstage = game->GetStage();
				state = Scene::CGameOver;
				ShowGameOver();
				delete game;
				break;
			}
			case Scene::CGameOver: {
				if (CGameOverInput())
					ShowGameOver();
				break;
			}
			case Scene::Playing2: {
				Game2* game;
				game = new Game2(7);
				game->MainGame(record);
				//ShowRank(game2);
				state = Scene::GameOver2;
				ShowGameOver();
				delete game;
				break;
			}
			case Scene::GameOver2: {
				if (GameOver2Input())
					ShowGameOver();;
				break;
			}
			}
		}
	}
}