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

	InputState() {
		moveleft = false;
		moveright = false;
		quickdrop = false;
		fastfall = false;
		rotateL = false;
		rotateR = false;
		sethold = false;
	}
};

class Input {
private:

public:
	InputState GetCommand();
};