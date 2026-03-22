#pragma once

#include"DxLib.h"
#include <vector>

struct InputState {
	bool moveleft;
	bool moveright;
	bool quickdrop;
	bool fastfall;
	bool rotateL;
	bool rotateR;
	bool sethold;
	bool rotBoardL;
	bool rotBoardR;
	bool rotBoardH;

	InputState() {
		moveleft = false;
		moveright = false;
		quickdrop = false;
		fastfall = false;
		rotateL = false;
		rotateR = false;
		sethold = false;
		rotBoardL = false;
		rotBoardR = false;
		rotBoardH = false;
	}
};

struct InputMenu {
	bool up;
	bool down;
	bool select;

	InputMenu() {
		up = false;
		down = false;
		select = false;
	}
};

class Input {
private:

public:
	InputState GetCommand();
	InputMenu GetMenuCommand();
};