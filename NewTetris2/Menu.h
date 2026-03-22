#pragma once

#include<iostream>
#include "DxLib.h"
#include "Style.h"
#include "Game.h"
#include "Game2.h"
#include "Challenge.h"
#include "Input.h"
#include "Record.h"

enum class Scene {
	Title,
	Playing,
	GameOver,
	Challenge,
	CGameOver,
	Playing2,
	GameOver2
};

class Menu {
private:
	Input input;
	Style style;
	Record record;

	int menuselect;
	InputMenu prevcommand;
	InputMenu nowcommand;

	Scene state;
	bool MenuInput();
	bool GameOverInput();
	bool GameOver2Input();
	bool CGameOverInput();
	void ShowMenu();
	void ShowGameOver();
	void ShowRank(Game& game);
public:
	Menu();
	void MenuMain();
};