#pragma once
#include<iostream>
#include "Style.h"

#define LEFTSIDE 350
#define UPSIDE 200
#define WIDTH 10  //盤面の横のマス数
#define HEIGHT 20  //盤面の縦のマス数
#define WALLTHICK 3
#define PIX 10  //1マスの辺の長さ
#define CENTER_X 6
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
	VoidMino = -1
};


extern int form[7][4][4];
//class Board;

class Mino {
private:
	int now_form[4][4]; //現在動かしているミノ
	int rotate_form[4][4];//回転後のミノ
	int now_mino_x, now_mino_y;
	int angle;
	TetroMino shape;
	TetroMino nextmino[15];
	TetroMino hold;
	int dx;
	int dy;

	void ReturnMino(TetroMino shape);
	TetroMino CreateMino();
	void Shuffle();
	int TF(int num);

public:
	Mino();
	void NewMino();
	void init_mino();
	void Hold();
	void CreateRotateMatrix(int r);
	void Calculate_Rotate_XY(int gap_x, int gap_y, int r);
	void RotateMino(int r);

	int GetMino(int x, int y) const;
	void SetMino(int x, int y, TetroMino type);
	int GetRotateMino(int x, int y) const;
	int GetX() const;
	int GetY() const;
	int GetDX() const;
	int GetDY() const;
	void SetX(int x);
	void SetY(int y);
	void Fall();
	TetroMino GetShape() const;
	void SetShape(TetroMino type);
	void ResetMino();
	TetroMino GetHold() const;
	void SetHold(TetroMino type);
	TetroMino GetNext(int idx) const;
	void SetNext(int idx, TetroMino type);
	
	void ShowHold(Style& style);
	void ShowNext(Style& style);
	void ShowMino(Style& style);

};

