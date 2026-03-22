#pragma once

#include<iostream>
#include <vector>
#include "DxLib.h"
#include "Style.h"


#define LEFTSIDE 350
#define UPSIDE 200
#define WIDTH 10  //盤面の横のマス数
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
#define ENEMY -1

using namespace std;

struct Offset {
	int x;
	int y;
};

class Mino;

class Board {
private:

protected:
	//int mino[HEIGHT + 2 + 2 * WALLTHICK][WIDTH + 2 * WALLTHICK];  //ミノの種類 0:無　1:壁 2～8:固定ミノ 9～15:今動かしているミノ
	vector<vector<int>> mino;
	bool isFixedMino(int x, int y);
	bool CheckRotate(int gap_x, int gap_y, int r, Mino& current);
	bool TouchOtherMinos(int x, int y);
	virtual bool isMino(int x, int y) { return false; }
public:
	Board();
	bool Update(Mino& current);
	bool process_EST(Mino& current);
	void process_EST2(Mino& current);
	bool CanFallShade(Mino& current);
	bool CanRotate(int r, Mino& current);
	bool TouchSideWall(int s, Mino& current);
	bool TouchUp(Mino& current);
	bool ClearLines(vector<bool>& deletelist);
	virtual bool IsPerfectClear();
	int SpinEnter(Mino& current);
	void FallLines(vector<bool>& deletelist);
	virtual void Show(Style& style);
	inline int GetMino(int x, int y) const { return mino[y][x]; }
	inline void SetMino(int x, int y, int type){ mino[y][x] = type; }
	virtual void Reset();
	virtual int IsBoardSpin(Mino& current) { return 0; }
	virtual void RotateBoard(){}
	virtual bool CanRotateBoard(Mino& current) { return false; }
	virtual void CreateRotateBoard(int r) {}
	virtual void ResetRotateBoard() {}
	virtual bool ClearLines(vector<bool>& deletehorizon, vector<bool>& deletevertical, int spinboard, Mino& current, vector<vector<bool>>& mem) { return false; }
	virtual void FallLines(vector<bool>& deletehorizon, vector<bool>& deletevertical) {}

	virtual int GetHEIGHT();
	virtual void ShowRotate(int r, int k, Style& style) {}
};
