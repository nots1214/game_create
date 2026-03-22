#include "Mino.h"

int shuffle[7];

int form[7][4][4] = {
	{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},//I
	{{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},//T
	{{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},//J
	{{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},//L
	{{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},//Z
	{{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},//S
	{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}//O
};


Mino::Mino() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			now_form[i][j] = VOID;
			rotate_form[i][j] = VOID;
		}
	}
	now_mino_x = CENTER_X;
	now_mino_y = UP_Y;
	shade_y = UP_Y;
	for (int i = 0; i < 15; i++) {
		nextmino[i] = TetroMino::VoidMino;
	}
	hold = TetroMino::VoidMino;
	shape = TetroMino::VoidMino;
	angle = 0;
	dx = 0;
	dy = 0;
}

void Mino::ResetMino() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			now_form[i][j] = VOID;
		}
	}
}
//void Mino::ResetShade() {
//	shade_y = now_mino_y;
//}
//
//void Mino::QuickDrop() {
//	now_mino_y = shade_y;
//}

//上部に移動
void Mino::ReturnMino(TetroMino shape) {
	now_mino_x = CENTER_X;
	now_mino_y = UP_Y;
	shade_y = UP_Y;
	angle = 0;
	int idx = static_cast<int>(shape);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			now_form[i][j] = form[idx][i][j];
		}
	}
}

//新しいテトリミノ生成
TetroMino Mino::CreateMino() {
	//now = rand() % 7;
	TetroMino newMino = nextmino[0];
	for (int i = 0; i < 14; i++) {
		nextmino[i] = nextmino[i + 1];
	}
	if (nextmino[7] == TetroMino::VoidMino) {
		Shuffle();
		for (int i = 0; i < 7; i++) {
			nextmino[7 + i] = static_cast<TetroMino>(shuffle[i]);
		}
	}
	return newMino;
}

//ホールド
void Mino::Hold() {
	TetroMino tmp;
	ResetMino();
	if (hold == TetroMino::VoidMino)//始めてホールドする時
	{
		hold = shape;
		shape = CreateMino();
	}
	else {//既にホールドしたことがあるとき
		tmp = shape;
		shape = hold;
		hold = tmp;
	}
	ReturnMino(shape);
}

//次のミノに切り替える
void Mino::NewMino() {
	shape = CreateMino();
	ReturnMino(shape);
}


//テトリミノのランダム生成
void Mino::Shuffle() {
	int tmp;
	int ran1, ran2;
	for (int i = 0; i < 7; i++) {
		shuffle[i] = i;
	}
	for (int i = 0; i < 20; i++) {
		ran1 = rand() % 7;
		ran2 = rand() % 6;
		ran2 = (ran1 + ran2) % 7;
		tmp = shuffle[ran1];
		shuffle[ran1] = shuffle[ran2];
		shuffle[ran2] = tmp;
	}
}

//初期ミノ生成
void Mino::init_mino() {
	SetNext(14, TetroMino::VoidMino);
	Shuffle();
	for (int i = 0; i < 7; i++) {
		SetNext(i, static_cast<TetroMino>(shuffle[i]));
	}
	Shuffle();
	for (int i = 0; i < 7; i++) {
		SetNext(i + 7, static_cast<TetroMino>(shuffle[i]));
	}

	NewMino();
}

void Mino::initChallenge(vector<TetroMino>& loc) {
	for (int i = 0; i < loc.size(); i++) {
		SetNext(i, static_cast<TetroMino>(loc[i]));
	}
	NewMino();
}

//多分TransFormの略
int Mino::TF(int num) {   //1→2,-1→1変換
	int bo;
	if (num == 1)
		bo = 2;
	else
		bo = 1;
	//return (3 + num) / 2;
	return bo;
}


//回転によってずれる座標計算
void Mino::Calculate_Rotate_XY(int gap_x, int gap_y, int r) {
	int c = 0;
	int theta = 0;
	if (shape != TetroMino::Imino) {
		if (angle % 2 == 0) {
			dx = gap_x * r * (1 - angle);
			dy = gap_y;
		}
		else {
			dx = -gap_x * (2 - angle);
			dy = -gap_y;
		}
	}
	else {
		if (angle % 2 == 0) {
			theta = 1 - angle;
			dx = gap_x * theta * TF(-r * gap_x);
			dy = gap_y * theta * r * TF(-r * gap_y);
		}
		else {
			theta = 2 - angle;
			if (theta == 1) {
				dx = gap_x * r * TF(r * gap_x);
				dy = -gap_y * TF(r * gap_y);
			}
			else if (theta == -1) {
				c = 2 * abs(gap_y - gap_x) - 3;
				dx = -gap_x * r * c * TF(r * gap_x * c);
				dy = gap_y * TF(r * gap_y);
			}
		}
	}
	return;
}


//回転後の状態作成
//Iミノだけ特殊
void Mino::CreateRotateMatrix(int r) {
	if (shape == TetroMino::Imino || shape == TetroMino::Omino) {//IミノかOミノ
		if (r == -1) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					rotate_form[3 - j][i] = now_form[i][j];
				}
			}
		}
		else if (r == 1) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					rotate_form[j][3 - i] = now_form[i][j];
				}
			}

		}
	}
	else {//SZLJTミノ
		if (r == -1) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					rotate_form[2 - j][i] = now_form[i][j];
				}
			}

		}
		else if (r == 1) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					rotate_form[j][2 - i] = now_form[i][j];
				}
			}

		}
		for (int i = 0; i < 4; i++) {
			rotate_form[i][3] = VOID;
			rotate_form[3][i] = VOID;
		}
	}
}

//回転処理
void Mino::RotateMino(int r) {
	//回転後行列生成→今の形にコピー→座標と方向計算
	CreateRotateMatrix(r);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			now_form[i][j] = rotate_form[i][j];
			rotate_form[i][j] = VOID;

		}
	}
	now_mino_x += dx;
	now_mino_y += dy;
	angle = (angle + r + 4) % 4;
}

void Mino::ShowHold(Style& style) {
	int stylecolor;

	stylecolor = style.GetStyle(static_cast<int>(GetHold()));
	DrawLine(HOLD_X, HOLD_Y, HOLD_X, HOLD_Y + 50, 0xFFFF);
	DrawLine(HOLD_X + 50, HOLD_Y, HOLD_X + 50, HOLD_Y + 50, 0xFFFF);
	DrawLine(HOLD_X, HOLD_Y, HOLD_X + 50, HOLD_Y, 0xFFFF);
	DrawLine(HOLD_X, HOLD_Y + 50, HOLD_X + 50, HOLD_Y + 50, 0xFFFF);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			DrawGraph(HOLD_X + i * PIX + 1, HOLD_Y + PIX * j + 1, style.GetVoidStyle(), TRUE);
		}
	}
	for (int i = 0; i < 4; i++) {
		DrawGraph(HOLD_X + 4 * PIX, HOLD_Y + PIX * i + 1, style.GetVoidStyle(), TRUE);
		DrawGraph(HOLD_X + i * PIX + 1, HOLD_Y + PIX * 4, style.GetVoidStyle(), TRUE);
	}
	DrawGraph(HOLD_X + 4 * PIX, HOLD_Y + PIX * 4, style.GetVoidStyle(), TRUE);
	if (GetHold() != TetroMino::VoidMino) {
		if (GetHold() == TetroMino::Imino || GetHold() == TetroMino::Omino)
		{
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					if (form[static_cast<int>(GetHold())][j][k])
					{
						DrawGraph(HOLD_X + 4 + k * (PIX + 1), HOLD_Y + 7 + j * (PIX + 1), stylecolor, TRUE);
					}
				}
			}
		}
		else {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (form[static_cast<int>(GetHold())][j][k]) {
						DrawGraph(HOLD_X + 10 + k * (PIX + 1), HOLD_Y + 13 + j * (PIX + 1), stylecolor, TRUE);
					}
				}
			}
		}
	}
	DrawGraph(HOLD_X + 5, HOLD_Y - 15, style.GetStringStyle(6), TRUE);
}

void Mino::ShowNext(Style& style) {
	int stylecolor;

	for (int i = 0; i < NEXT_NUM; i++) {
		DrawLine(NEXT_X, NEXT_Y + i * 60, NEXT_X, NEXT_Y + 50 + i * 60, 0xFFFF);
		DrawLine(NEXT_X + 50, NEXT_Y + i * 60, NEXT_X + 50, NEXT_Y + 50 + i * 60, 0xFFFF);
		DrawLine(NEXT_X, NEXT_Y + i * 60, NEXT_X + 50, NEXT_Y + i * 60, 0xFFFF);
		DrawLine(NEXT_X, NEXT_Y + 50 + i * 60, NEXT_X + 50, NEXT_Y + 50 + i * 60, 0xFFFF);
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				DrawGraph(NEXT_X + k * PIX + 1, NEXT_Y + j * PIX + 1 + i * 60, style.GetVoidStyle(), TRUE);
			}
		}
		for (int j = 0; j < 4; j++) {
			DrawGraph(NEXT_X + 4 * PIX, NEXT_Y + j * PIX + 1 + i * 60, style.GetVoidStyle(), TRUE);
			DrawGraph(NEXT_X + j * PIX + 1, NEXT_Y + 4 * PIX + i * 60, style.GetVoidStyle(), TRUE);
		}
		DrawGraph(NEXT_X + 4 * PIX, NEXT_Y + 4 * PIX + i * 60, style.GetVoidStyle(), TRUE);
		if (GetNext(i) == TetroMino::Imino || GetNext(i) == TetroMino::Omino)
		{
			stylecolor = style.GetStyle(static_cast<int>(GetNext(i)));
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					if (form[static_cast<int>(GetNext(i))][j][k])
					{
						DrawGraph(NEXT_X + 4 + k * (PIX + 1), NEXT_Y + 7 + i * 60 + j * (PIX + 1), stylecolor, TRUE);
					}
				}
			}
		}
		else {
			stylecolor = style.GetStyle(static_cast<int>(GetNext(i)));
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (form[static_cast<int>(GetNext(i))][j][k]) {
						DrawGraph(NEXT_X + 10 + k * (PIX + 1), NEXT_Y + 13 + i * 60 + j * (PIX + 1), stylecolor, TRUE);
					}
				}
			}
		}
	}
	DrawGraph(NEXT_X + 5, NEXT_Y - 15, style.GetStringStyle(5), TRUE);
}

void Mino::ShowShade(Style& style) {
	int stylecolor = style.GetStyle(static_cast<int>(shape)+8);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (now_form[i][j])
			{
				DrawGraph(LEFTSIDE + (j + now_mino_x - 3) * (PIX + 1) + 1, UPSIDE + (i + shade_y - 5) * (PIX + 1) + 1, stylecolor, TRUE);
			}
		}
	}
}

void Mino::ShowMino(Style& style) {
	int stylecolor;

	stylecolor = style.GetStyle(static_cast<int>(shape));
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (GetMino(j, i)) {
				DrawGraph(LEFTSIDE + (j+now_mino_x - 3) * (PIX + 1) + 1, UPSIDE + (i+now_mino_y - 5) * (PIX + 1) + 1, stylecolor, TRUE);
			}

		}
	}
}