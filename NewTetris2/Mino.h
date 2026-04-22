#pragma once
#include<iostream>
#include "Style.h"
#include<vector>

using namespace std;

#define LEFTSIDE 350
#define UPSIDE 200
#define WALLTHICK 3
#define PIX 10  //1マスの辺の長さ
//#define CENTER_X 6
#define UP_Y 4
#define NEXT_X 500
#define NEXT_Y 150
#define NEXT_NUM 5
#define HOLD_X 280
#define HOLD_Y 180
#define VOID 0

enum class TetroMino {
	Imino = 0,
	Tmino = 1,
	Jmino = 2,
	Lmino = 3,
	Zmino = 4,
	Smino = 5,
	Omino = 6,
	VoidMino = -1,
	UnusedMino = -2
};


extern int form[7][4][4];

class Mino {
private:
	int now_form[4][4]; //現在動かしているミノ
	int rotate_form[4][4];//回転後のミノ
	int now_mino_x, now_mino_y;
	int shade_y;
	int angle;
	TetroMino shape;
	TetroMino nextmino[15];
	TetroMino hold;
	int dx;
	int dy;
	int CENTER_X;

	void ReturnMino(TetroMino shape);
	TetroMino CreateMino();
	void Shuffle();
	int TF(int num);

public:
	Mino(int cx);
	void NewMino();
	void init_mino();
	void initChallenge(vector<TetroMino>& loc);
	void Hold();
	void CreateRotateMatrix(int r);
	void Calculate_Rotate_XY(int gap_x, int gap_y, int r);
	void RotateMino(int r);

	inline bool GetMino(int x, int y) const { return now_form[y][x] == 1; }
	inline void SetMino(int x, int y, TetroMino type) { now_form[y][x] = static_cast<int>(type); }
	inline int GetRotateMino(int x, int y) const { return rotate_form[y][x]; }
	inline int GetX() const { return now_mino_x; }
	inline int GetY() const { return now_mino_y; }
	inline int GetShadeY() const { return shade_y; }
	inline int GetDX() const { return dx; }
	inline int GetDY() const { return dy; }
	inline void SetX(int x) { now_mino_x = x; }
	inline void SetY(int y) { now_mino_y = y; }
	inline void Fall() { now_mino_y++; }
	inline void ShadeFall() { shade_y++; }
	inline void QuickDrop() { now_mino_y = shade_y; }
	inline TetroMino GetShape() const { return shape; }
	inline void SetShape(TetroMino type) { shape = type; }
	void ResetMino();
	inline void ResetShade() { shade_y = now_mino_y; }
	inline TetroMino GetHold() const { return hold; }
	inline void SetHold(TetroMino type) { hold = type; }
	inline TetroMino GetNext(int idx) const { return nextmino[idx]; }
	inline void SetNext(int idx, TetroMino type) { nextmino[idx] = type; }
	
	void ShowHold(Style& style);
	void ShowNext(Style& style);
	void ShowShade(Style& style);
	void ShowMino(Style& style);

};

