#include "Input.h"

InputState Input::GetCommand() {
	InputState s{};
	s.moveleft = CheckHitKey(KEY_INPUT_A);
	s.moveright = CheckHitKey(KEY_INPUT_D);
	s.quickdrop = CheckHitKey(KEY_INPUT_W);
	s.fastfall = CheckHitKey(KEY_INPUT_S);
	s.rotateL = CheckHitKey(KEY_INPUT_O);
	s.rotateR = CheckHitKey(KEY_INPUT_P);
	s.sethold = CheckHitKey(KEY_INPUT_I);
	s.rotBoardL = CheckHitKey(KEY_INPUT_K);
	s.rotBoardR = CheckHitKey(KEY_INPUT_L);
	s.rotBoardH = CheckHitKey(KEY_INPUT_J);
	return s;
}

InputMenu Input::GetMenuCommand() {
	InputMenu s{};
	s.up = CheckHitKey(KEY_INPUT_UP);
	s.down = CheckHitKey(KEY_INPUT_DOWN);
	s.select = CheckHitKey(KEY_INPUT_SPACE);
	return s;
}