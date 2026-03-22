#include "Board2.h"
#include "Mino.h"

const int OFFSET_Y = WALLTHICK + 2;
const int OFFSET_X = WALLTHICK;


const Offset diffTable[2][5] = {
	{{0, 0},{-1, 0},{-1, -1},{0, 2},{-1, 2}},
	{{0, 0},{-1, 0},{1, 0},{-1, 1},{1, -1}}
};

Board2::Board2()
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

	rotate_board.resize(GetHEIGHT(), vector<int>(WIDTH));
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			rotate_board[i][j] = VOID;
		}
	}
}

int Board2::GetHEIGHT() { return 10; }

//他のミノと接触しているか
bool Board2::isMino(int x, int y) {
	return FIXEDI <= GetMino(x, y) && GetMino(x, y) <= FIXEDO;
}

//回転技
int Board2::IsBoardSpin(Mino& current) {
	int bspincnt = 0;;
	if (current.GetShape() == TetroMino::Imino || current.GetShape() == TetroMino::Omino) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (TouchOtherMinos(current.GetX() + j, current.GetY() + i))
					bspincnt++;
			}
		}
		if (bspincnt >= 9) return static_cast<int>(current.GetShape());
		else return -1;
	}
	else {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (TouchOtherMinos(current.GetX() + j, current.GetY() + i))
					bspincnt++;
			}
		}
		if (bspincnt >= 5) return static_cast<int>(current.GetShape());
		else return -1;
	}
}

//回転できるかチェック
bool Board2::CanRotateBoard(Mino& current) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (current.GetMino(j, i))
			{
				if (0 <= current.GetY() - OFFSET_Y + i && current.GetY() - OFFSET_Y + i<GetHEIGHT() && 0 <= current.GetX() - OFFSET_X + j && current.GetX() - OFFSET_X + j < WIDTH)
				{
					if (FIXEDI <= rotate_board[current.GetY() - OFFSET_Y + i][current.GetX() - OFFSET_X + j] && rotate_board[current.GetY() - OFFSET_Y + i][current.GetX() - OFFSET_X + j] <= FIXEDO)
						return false;
				}
			}
		}
	}
	return true;
}

//配列初期化
void Board2::ResetRotateBoard() {
	for (int i = 0; i < GetHEIGHT(); i++) {
		for (int j = 0; j < WIDTH; j++) {
			rotate_board[i][j] = VOID;
		}
	}
}

//回転後の盤面を生成
void Board2::CreateRotateBoard(int r) {
	if (r == 1) {//右回転
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < GetHEIGHT(); j++) {
				rotate_board[j][9 - i] = mino[i + OFFSET_Y][j + OFFSET_X];
			}
		}
	}
	else if(r==-1){//左回転
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < GetHEIGHT(); j++) {
				rotate_board[9 - j][i] = mino[i + OFFSET_Y][j + OFFSET_X];
			}
		}
	}
	else {
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < GetHEIGHT(); j++) {
				rotate_board[9 - i][9 - j] = mino[i + OFFSET_Y][j + OFFSET_X];
			}
		}
	}
}

//盤面回転
void Board2::RotateBoard() {
	for (int i = 0; i < GetHEIGHT(); i++) {
		for (int j = 0; j < WIDTH; j++) {
			mino[i+OFFSET_Y][j+OFFSET_X] = rotate_board[i][j];
		}
	}
}


//ライン消去
bool Board2::ClearLines(vector<bool>& deletehorizon, vector<bool>& deletevertical,int spinboard, Mino& current, vector<vector<bool>>& mem) {
	bool canDelete = false;
	for (int i = 0; i < GetHEIGHT(); i++) {
		deletehorizon[i] = true;
		for (int j = 0; j < WIDTH; j++) {
			if (!isFixedMino(j + OFFSET_X, i + OFFSET_Y))
			{
				deletehorizon[i] = false;
				break;
			}
		}
		if (deletehorizon[i])
			canDelete = true;
	}

	for (int i = 0; i < WIDTH; i++) {
		deletevertical[i] = true;
		for (int j = 0; j < GetHEIGHT(); j++) {
			if (!isFixedMino(i + OFFSET_X, j + OFFSET_Y))
			{
				deletevertical[i] = false;
				break;
			}
		}
		if (deletevertical[i])
			canDelete = true;
	}

	if (spinboard != -1)
	{
		if(current.GetShape() == TetroMino::Imino || current.GetShape() == TetroMino::Omino)
		{
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (mem[i][j]) {
						if (current.GetY() + i - OFFSET_Y < 10)
							deletehorizon[current.GetY() + i - OFFSET_Y] = true;
						if (current.GetX() + j - OFFSET_X < 10)
							deletevertical[current.GetX() + j - OFFSET_X] = true;
					}
				}
			}
		}
		else {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (mem[i][j]) {
						if (current.GetY() + i - OFFSET_Y < 10)
							deletehorizon[current.GetY() + i - OFFSET_Y] = true;
						if (current.GetX() + j - OFFSET_X< 10)
							deletevertical[current.GetX() + j - OFFSET_X] = true;
					}
				}
			}
		}
		canDelete = true;
	}

	return canDelete;
}

//ライン消去のあと、上部のミノは落ちる
void Board2::FallLines(vector<bool>& deletehorizon, vector<bool>& deletevertical) {
	for (int i = 0; i < GetHEIGHT(); i++) {
		if (deletehorizon[i])
		{
			for (int j = 0; j < WIDTH; j++) {
				mino[i + OFFSET_Y][j + OFFSET_X] = VOID;
				for (int k = i; k > -2; k--) {
					mino[k + OFFSET_Y][j + OFFSET_X] = mino[k + OFFSET_Y - 1][j + OFFSET_X];
				}
			}
		}
	}

	for (int i = 0; i < WIDTH; i++) {
		if (deletevertical[i])
		{
			for (int j = 0; j < GetHEIGHT(); j++) {
				mino[j + OFFSET_Y][i + OFFSET_X] = VOID;
			/*	for (int k = i; k > -1; k--) {
					if (isMino(k + OFFSET_X - 1, j + OFFSET_Y))
						mino[j + OFFSET_Y][k + OFFSET_X] = mino[j + OFFSET_Y][k + OFFSET_X - 1];
					else
						mino[j + OFFSET_Y][k + OFFSET_X] = VOID;
				}*/
			}
		}
	}
}


//重心　LESTSIDE+5*(PIX+1),UPSIDE + 5*(PIX+1)
//現在のx座標　LEFTSIDE + i*(PIX+1)
//差(i-5)*(PIX+1)
void Board2::ShowRotate(int r, int k, Style& style) {
	//背景
	DrawGraph(0, 0, style.GetBackStyle(), TRUE);
	
	int vx1, vy1, vx2, vy2, vx3, vy3;
	int mx = LEFTSIDE + GetHEIGHT() / 2 * (PIX + 1);
	int my = UPSIDE + GetHEIGHT() / 2 * (PIX + 1);
	double rad;
	if (r != 0)
		rad = (double)(k * r) * DX_PI / 20.0;
	else
		rad = (double(k)) * DX_PI / 10.0;
	
	//ミノ表示
	for (int i = 0; i < GetHEIGHT(); i++) {
		for (int j = 0; j < WIDTH; j++) {
			vx1 = (j - 5) * (PIX + 1) + 1 + 5;
			vy1 = (i - 5) * (PIX + 1) + 1 + 5;
			vx2 = (int)(cos(rad) * vx1 - sin(rad) * vy1);
			vy2 = (int)(sin(rad) * vx1 + cos(rad) * vy1);
			if (GetMino(OFFSET_X + j, OFFSET_Y + i) > 0) {
				DrawRotaGraph(mx + vx2, my + vy2, 1.0, rad, style.GetStyle(GetMino(OFFSET_X + j, OFFSET_Y + i) - FIXEDI), TRUE);
			}
			else if (GetMino(OFFSET_X + j, OFFSET_Y + i) == -1) {
				DrawRotaGraph(mx + vx2, my + vy2, 1.0, rad, style.GetStyle(15), TRUE);
			}
			else DrawRotaGraph(mx + vx2, my + vy2, 1.0, rad, style.GetVoidStyle(), TRUE);
		}
	}


	for (int i = 0; i < WIDTH + 1; i++) {
		vx1 = (i - 5) * (PIX + 1);
		vy1 = -5 * (PIX + 1);
		vx2 = (int)(cos(rad) * vx1 - sin(rad) * vy1);
		vy2 = (int)(sin(rad) * vx1 + cos(rad) * vy1);
		vx3 = (int)(cos(rad) * vx1 + sin(rad) * vy1);
		vy3 = (int)(sin(rad) * vx1 - cos(rad) * vy1);
		DrawLine(mx + vx2, my + vy2, mx + vx3, my + vy3, 0xFFFFFF);
	}
	for (int i = 0; i < GetHEIGHT() + 1; i++) {
		vx1 = -5 * (PIX + 1);
		vy1 = (i - 5) * (PIX + 1);
		vx2 = (int)(cos(rad) * vx1 - sin(rad) * vy1);
		vy2 = (int)(sin(rad) * vx1 + cos(rad) * vy1);
		vx3 = (int)( - cos(rad) * vx1 - sin(rad) * vy1);
		vy3 = (int)( - sin(rad) * vx1 + cos(rad) * vy1);
		DrawLine(mx + vx2, my + vy2, mx + vx3, my + vy3, 0xFFFFFF);
	}
}