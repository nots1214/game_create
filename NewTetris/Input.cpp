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
	return s;
}