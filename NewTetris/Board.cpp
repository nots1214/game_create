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
	for (int i = OFFSET_Y-2; i < HEIGHT + OFFSET_Y; i++)
		for (int j = OFFSET_X; j < WIDTH + OFFSET_X; j++)
			mino[i][j] = VOID;

	for (int i = 0; i < WIDTH + 2 * WALLTHICK; i++)
	{
		mino[0][i] = WALL;
		mino[1][i] = WALL;
		mino[2][i] = WALL;
		mino[25][i] = WALL;
		mino[26][i] = WALL;
		mino[27][i] = WALL;
	}
	for (int i = 0; i < HEIGHT + 2 * WALLTHICK; i++)
	{
		mino[i][0] = WALL;
		mino[i][1] = WALL;
		mino[i][2] = WALL;
		mino[i][13] = WALL;
		mino[i][14] = WALL;
		mino[i][15] = WALL;
	}//壁判定
	deletelist.assign(20, false);
}


int Board::GetMino(int x, int y) const
{
	return mino[y][x];
}

void Board::SetMino(int x, int y, int type) {
	mino[y][x] = type;
}

bool Board::TouchOtherMinos(int x, int y) {
	return ((FIXEDI <= GetMino(x, y) && GetMino(x, y) <= FIXEDO) || GetMino(x, y) == WALL);
}


bool Board::isFixedMino(int x, int y) {
	return (FIXEDI <= GetMino(x, y) && GetMino(x, y) <= FIXEDO);
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
bool Board::ClearLines() {
	bool canDelete = false;
	for (int i = 0; i < HEIGHT; i++) {
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
	for (int i = 0; i < HEIGHT; i++) {
		if (deletelist[i])
		{
			for (int j = 0; j < WIDTH; j++) {
				mino[i + OFFSET_Y][j + OFFSET_X] = VOID;
				for (int k = i; k > 0; k--) { //画面外上側の処理はまだ
					mino[k + OFFSET_Y][j + OFFSET_X] = mino[k + OFFSET_Y - 1][j + OFFSET_X];
				}
			}

		}
	}
	return canDelete;
}


//表示
void Board::Show(Style& style){

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (GetMino(OFFSET_X + j, OFFSET_Y + i)) {
				DrawGraph(LEFTSIDE + j * (PIX + 1) + 1, UPSIDE + i * (PIX + 1) + 1, style.GetStyle(GetMino(OFFSET_X + j, OFFSET_Y + i) - FIXEDI), TRUE);
			}
		}
	}
	for (int i = 0; i < WIDTH + 1; i++) {
		DrawLine(LEFTSIDE + i * (PIX + 1), UPSIDE, LEFTSIDE + i * (PIX + 1), UPSIDE + HEIGHT * (PIX + 1), 0xFFFFFF);
	}
	for (int i = 0; i < HEIGHT + 1; i++) {
		DrawLine(LEFTSIDE, 200 + i * (PIX + 1), LEFTSIDE + WIDTH * (PIX + 1), UPSIDE + i * (PIX + 1), 0xFFFFFF);
	}

	//DrawFormatString(400, 0, 0xffffff, "%d", current.GetHold());
}