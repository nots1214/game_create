#pragma once

#include<iostream>
#include <vector>
#include "DxLib.h"
#include "Style.h"

#define LEFTSIDE 350
#define UPSIDE 200
#define WIDTH 10  //盤面の横のマス数
#define HEIGHT 20  //盤面の縦のマス数
#define WALLTHICK 3
#define PIX 10  //1マスの辺の長さ
#define VOID 0
#define WALL 1
#define FIXEDI 2
#define FIXEDT 3
#define FIXEDJ 4
#define FIXEDL 5
#define FIXEDZ 6
#define FIXEDS 7
#define FIXEDO 8

using namespace std;

struct Offset {
	int x;
	int y;
};

class Mino;

class Board {
private:
	int mino[HEIGHT + 2 + 2 * WALLTHICK][WIDTH + 2 * WALLTHICK];  //ミノの種類 0:無　1:壁 2～8:固定ミノ 9～15:今動かしているミノ
	vector<bool> deletelist;
	bool isFixedMino(int x, int y);
	bool CheckRotate(int gap_x, int gap_y, int r, Mino& current);
	bool TouchOtherMinos(int x, int y);
public:
	Board();
	bool Update(Mino& current);
	bool process_EST(Mino& current);
	void process_EST2(Mino& current);
	bool CanRotate(int r, Mino& current);
	bool TouchSideWall(int s, Mino& current);
	bool ClearLines();
	void Show(Style& style);
	int GetMino(int x, int y) const;
	void SetMino(int x, int y, int shape);
};
