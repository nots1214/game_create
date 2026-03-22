#include "Board.h"
#include "Mino.h"

const int OFFSET_Y = WALLTHICK + 2;
const int OFFSET_X = WALLTHICK;


const Offset diffTable[2][5] = {
	{{0, 0},{-1, 0},{-1, -1},{0, 2},{-1, 2}},
	{{0, 0},{-1, 0},{1, 0},{-1, 1},{1, -1}}
};

Board::Board()
{

	mino.resize(GetHEIGHT() + 2 + 2 * WALLTHICK, vector<int>(WIDTH + 2 * WALLTHICK));
	for (int i = OFFSET_Y - 2; i < GetHEIGHT() + OFFSET_Y; i++)
		for (int j = OFFSET_X; j < WIDTH + OFFSET_X; j++)
			mino[i][j] = VOID;

	for (int i = 0; i < WIDTH + 2 * WALLTHICK; i++)
	{
		mino[0][i] = WALL;
		mino[1][i] = WALL;
		mino[2][i] = WALL;
		mino[GetHEIGHT() + 5][i] = WALL;
		mino[GetHEIGHT() + 6][i] = WALL;
		mino[GetHEIGHT() + 7][i] = WALL;
	}
	for (int i = 0; i < GetHEIGHT() + 2 * WALLTHICK; i++)
	{
		mino[i][0] = WALL;
		mino[i][1] = WALL;
		mino[i][2] = WALL;
		mino[i][13] = WALL;
		mino[i][14] = WALL;
		mino[i][15] = WALL;
	}//壁判定
}

int Board::GetHEIGHT() { return 20; }

void Board::Reset() {
	for (int i = OFFSET_Y; i < GetHEIGHT() + OFFSET_Y; i++) {
		for (int j = OFFSET_X; j < WIDTH + OFFSET_X; j++) {
			SetMino(j, i, VOID);
		}
	}
}

bool Board::TouchOtherMinos(int x, int y) {
	return ((FIXEDI <= GetMino(x, y) && GetMino(x, y) <= FIXEDO) || GetMino(x, y) == WALL || GetMino(x,y) == ENEMY);
}


bool Board::isFixedMino(int x, int y) {
	return (FIXEDI <= GetMino(x, y) && GetMino(x, y) <= FIXEDO || GetMino(x, y) == ENEMY);
}

//回転できるか
bool Board::CheckRotate(int gap_x, int gap_y, int r, Mino& current) {

	current.Calculate_Rotate_XY(gap_x, gap_y, r);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (current.GetRotateMino(j,i)) {
				if (TouchOtherMinos(current.GetX() + j + current.GetDX(), current.GetY() + i + current.GetDY()))
					return false;
			}
		}
	}
	return true;

}

//ずらして回転できるか
bool Board::CanRotate(int r,Mino& current) {
	//回転後行列生成→その行列に対して5回位置をずらして置ける場所を探索
	current.CreateRotateMatrix(r);
	bool CanRotate = false;
	int IsShapeI = (current.GetShape() == TetroMino::Imino) ? 1 : 0;
	for (int i = 0; i < 5; i++) {
		Offset o = diffTable[IsShapeI][i];
		if (CheckRotate(o.x, o.y, r, current)) {
			CanRotate = true;
			break;
		}
	}

	return CanRotate;
}

//壁判定
bool Board::TouchSideWall(int s, Mino& current) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (current.GetMino(j, i)) {
				if (TouchOtherMinos(current.GetX() + j + s, current.GetY() + i))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//Game Over判定
bool Board::TouchUp(Mino& current) {
	//上部に触れたら
	for (int i = 3; i < OFFSET_Y; i++) {
		for (int j = OFFSET_X; j < WIDTH + OFFSET_X; j++) {
			if (isFixedMino(j,i)) {
				return true;
			}
		}
	}

	//出現位置に他のミノがあったら
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (current.GetMino(j, i) && isFixedMino(current.GetX() + j, current.GetY() + i))
				return true;
		}
	}
	return false;
}


//盤面更新
//下にミノがあれば設置(TRUE)、なければ落下(FALSE)
bool Board::Update(Mino& current) {
	if (!process_EST(current))
	{
		current.Fall();
		return false;
	}
	else
	{
		process_EST2(current);
		return true;
	}
}

//下にミノがあるか判定
bool Board::process_EST(Mino& current) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (current.GetMino(j, i)) {
				if (TouchOtherMinos(current.GetX() + j, current.GetY() + i + 1))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//影の設定
bool Board::CanFallShade(Mino& current) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (current.GetMino(j, i)) {
				if (TouchOtherMinos(current.GetX() + j, current.GetShadeY() + i + 1)) {
					return true;
				}
			}
		}
	}
	return false;
}

//床設置判定
void Board::process_EST2(Mino& current) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (current.GetMino(j, i)) {
				SetMino(current.GetX() + j, current.GetY() + i, static_cast<int>(current.GetShape()) + FIXEDI);
			}
		}
	}
}

//ライン消去
bool Board::ClearLines(vector<bool>& deletelist) {
	bool canDelete = false;
	for (int i = 0; i < GetHEIGHT(); i++) {
		deletelist[i] = true;
		for (int j = 0; j < WIDTH; j++) {
			if (!isFixedMino(j + OFFSET_X, i + OFFSET_Y))
			{
				deletelist[i] = false;
				break;
			}
		}
		if (deletelist[i])
			canDelete = true;
	}
	return canDelete;
}

//ライン消去のあと、上部のミノは落ちる
void Board::FallLines(vector<bool>& deletelist) {
	for (int i = 0; i < GetHEIGHT(); i++) {
		if (deletelist[i])
		{
			for (int j = 0; j < WIDTH; j++) {
				mino[i + OFFSET_Y][j + OFFSET_X] = VOID;
				for (int k = i; k > -2; k--) {
					mino[k + OFFSET_Y][j + OFFSET_X] = mino[k + OFFSET_Y - 1][j + OFFSET_X];
				}
			}
		}
	}
}

//全消し判定
bool Board::IsPerfectClear() {
	for (int i = 3; i < GetHEIGHT() + OFFSET_Y; i++) {
		for (int j = OFFSET_X; j < WIDTH + OFFSET_X; j++) {
			if (isFixedMino(j, i))
				return false;
		}
	}
	return true;
}


//回転入れ判定
int Board::SpinEnter(Mino& current) {
	int fixcnt = 0;
	if (current.GetShape() != TetroMino::Imino && current.GetShape() != TetroMino::Omino)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (TouchOtherMinos(current.GetX() + j, current.GetY() + i)) {
					fixcnt++;
				}
			}
		}
		if (fixcnt >= 3)
			return static_cast<int>(current.GetShape());
		else
			return -1;
	}
	else
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (TouchOtherMinos(current.GetX() + j, current.GetY() + i)) {
					fixcnt++;
				}
			}
		}
		if (fixcnt >= 6)
			return static_cast<int>(current.GetShape());
		else
			return -1;
	}
}

//表示
void Board::Show(Style& style){
	//背景
	DrawGraph(0, 0, style.GetBackStyle(), TRUE);
	//ミノ表示
	for (int i = 0; i < GetHEIGHT(); i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (GetMino(OFFSET_X + j, OFFSET_Y + i)>0) {
				DrawGraph(LEFTSIDE + j * (PIX + 1) + 1, UPSIDE + i * (PIX + 1) + 1, style.GetStyle(GetMino(OFFSET_X + j, OFFSET_Y + i) - FIXEDI), TRUE);
			}
			else if (GetMino(OFFSET_X + j, OFFSET_Y + i) == -1) {
				DrawGraph(LEFTSIDE + j * (PIX + 1) + 1, UPSIDE + i * (PIX + 1) + 1, style.GetStyle(15), TRUE);
			}
			else DrawGraph(LEFTSIDE + j * (PIX + 1) + 1, UPSIDE + i * (PIX + 1) + 1, style.GetVoidStyle(),TRUE);
		}
	}
	//線
	for (int i = 0; i < WIDTH + 1; i++) {
		DrawLine(LEFTSIDE + i * (PIX + 1), UPSIDE, LEFTSIDE + i * (PIX + 1), UPSIDE + GetHEIGHT() * (PIX + 1), 0xFFFFFF);
	}
	for (int i = 0; i < GetHEIGHT() + 1; i++) {
		DrawLine(LEFTSIDE, 200 + i * (PIX + 1), LEFTSIDE + WIDTH * (PIX + 1), UPSIDE + i * (PIX + 1), 0xFFFFFF);
	}
}