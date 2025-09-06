

#include<stdio.h>
#include<string>
#include<iostream>
#include<windows.h>
#include<math.h>
#include<stdlib.h>
#include"DxLib.h"

using namespace std;

#define WIDTH 10  //盤面の横のマス数
#define HIGHT 20  //盤面の縦のマス数
#define PIX 10  //1マスの辺の長さ
#define AIR 0
#define SKY 1
#define PURPLE 2
#define BLUE 3
#define ORANGE 4
#define RED 5
#define GREEN 6
#define YELLOW 7
#define GLEY 8
#define GRA 1000
#define FRAME 50
#define NEXT_X 600
#define NEXT_Y 150
#define NEXT_NUM 5
#define HOLD_X 380
#define HOLD_Y 180

int mino[28][16] = { 0 }; //ミノの種類 0:無　1:ミノ　2:壁 3:今動かしているミノ
int form[7][4][4] = {
	{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},//I
	{{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},//T
	{{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},//J
	{{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},//L
	{{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},//Z
	{{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},//S
	{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}//O
};
int now_form[4][4] = { 0 };
int rotate_form[4][4] = { 0 };

int gra_t = 0;
int wait_t = 0;
int now = 0;
int now_mino_x = 6;
int now_mino_y = 4;
int status; //0:待ち時間 1:落下中
char color[7][10] = { "Imino.png","Tmino.png" ,"Jmino.png" ,"Lmino.png" ,"Zmino.png" ,"Smino.png" ,"Omino.png" };
int deletelist[20] = { 0 };
int nextmino[15] = { 0 };
int shuffle[7];
int hold=-1;
int holdable = 1;
int angle;
int rotate_check;
int theta=0;
int c = 0;


int TF(int num) {   //1→2,-1→1
	int bo;
	if (num == 1)
		bo = 2;
	else
		bo = 1;
	//return (3 + num) / 2;
	return bo;
}

int  CheckRotate(int gap_x,int gap_y,int r) {
	//int c=0;
	if (now!=0) {
		if (angle % 2 == 0) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (rotate_form[i][j]) {
						if (mino[now_mino_y + i + gap_y][now_mino_x + j + gap_x * r * (1 - angle)] == 1 || mino[now_mino_y + i + gap_y][now_mino_x + j + gap_x * r * (1 - angle)] == 2)
							return 0;
					}
				}
			}
			now_mino_x += gap_x * r * (1 - angle);
			now_mino_y += gap_y;
		}
		else {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (rotate_form[i][j]) {
						if (mino[now_mino_y + i - gap_y][now_mino_x + j - gap_x * (2 - angle)] == 1 || mino[now_mino_y + i - gap_y][now_mino_x + j - gap_x * (2 - angle)] == 2)
							return 0;
					}
				}
			}
			now_mino_x -= gap_x * (2 - angle);
			now_mino_y -= gap_y;
		}
	}
	else {
		if (angle % 2 == 0) {
			theta = 1 - angle;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (rotate_form[i][j]) {
						if (mino[now_mino_y + i + gap_y*theta*r*TF(-r*gap_y)][now_mino_x + j + gap_x * theta * TF(-r * gap_x)] == 1 || mino[now_mino_y + i + gap_y * theta * r * TF( - r * gap_y)][now_mino_x + j + gap_x * theta * TF(- r * gap_x)] == 2)
							return 0;
					}
				}
			}
			now_mino_x += gap_x * theta * TF(-r * gap_x);
			now_mino_y += gap_y * theta * r * TF(-r * gap_y);
		}
		else {
			theta = 2 - angle;
			if (theta == 1) {
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						if (rotate_form[i][j]) {
							if (mino[now_mino_y + i - gap_y * TF(r * gap_y)][now_mino_x + j + gap_x * r * TF(r * gap_x)] == 1 || mino[now_mino_y + i - gap_y * TF(r * gap_y)][now_mino_x + j + gap_x * r * TF(r * gap_x)] == 2)
								return 0;
						}
					}
				}
				now_mino_x += gap_x * r * TF(r * gap_x);
				now_mino_y -= gap_y * TF(r * gap_y);
			}
			else if(theta == -1){
				c = 2 * abs(gap_y - gap_x) - 3;
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						if (rotate_form[i][j]) {
							if (mino[now_mino_y + i + gap_y * TF(r * gap_y)][now_mino_x + j - gap_x * r *c* TF(r * gap_x*c)] == 1 || mino[now_mino_y + i + gap_y * TF(r * gap_y)][now_mino_x + j - gap_x * r *c* TF(r * gap_x*c)] == 2)
								return 0;
						}
					}
				}
				now_mino_x -= gap_x * r * c * TF(r * gap_x * c);
				now_mino_y += gap_y * TF(r * gap_y);
			}
		}
	}
	return 1;

}



void Shuffle() {
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
void RotateMino(int r) {
	rotate_check = 0;
	if (now == 0 || now == 6) {
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
	else {
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
	}
	//theta = 1 - angle;
	//check
	if (now != 0) {
		if (CheckRotate(0, 0, r)) {
			rotate_check = 1;
		}
		else if (CheckRotate(-1, 0, r)) {
			rotate_check = 1;
			if (angle % 2 == 0) {
			//	now_mino_x -= r * (1 - angle);
			}
			else {
			//	now_mino_x += (2 - angle);
			}
		}
		else if (CheckRotate(-1, -1, r)) {
			rotate_check = 1;
			if (angle % 2 == 0) {
			//	now_mino_x -= r * (1 - angle);
			//	now_mino_y--;
			}
			else {
			//	now_mino_x += (2 - angle);
			//	now_mino_y++;
			}

		}
		else if (CheckRotate(0, 2, r)) {
			rotate_check = 1;
			if (angle % 2 == 0) {
			//	now_mino_y += 2;
			}
			else {
			//	now_mino_y -= 2;
			}

		}
		else if (CheckRotate(-1, 2, r)) {
			rotate_check = 1;
			if (angle % 2 == 0) {
			//	now_mino_x -= r * (1 - angle);
			//	now_mino_y += 2;
			}
			else {
			//	now_mino_x += (2 - angle);
			//	now_mino_y -= 2;
			}
		}
		else {
			rotate_check = 0;
		}
	}
	else {
		if (CheckRotate(0, 0, r)) {
			rotate_check = 1;
		}
		else if (CheckRotate(-1, 0, r)) {
			rotate_check = 1;
			if (angle % 2 == 0) {
				//now_mino_x;
			}
			else {
			//	now_mino_x;
			}
		}
		else if (CheckRotate(1, 0, r)) {
			rotate_check = 1;
			if (angle % 2 == 0) {
			//	now_mino_x;
			}
			else {
			//	now_mino_x;
			}

		}
		else if (CheckRotate(-1, 1, r)) {
			rotate_check = 1;
			if (angle % 2 == 0) {
			//	now_mino_x;
			//	now_mino_y;
			}
			else {
			//	now_mino_x;
			//	now_mino_y;
			}

		}
		else if (CheckRotate(1, -1, r)) {
			rotate_check = 1;
			if (angle % 2 == 0) {
			//	now_mino_x;
			//	now_mino_y;
			}
			else {
				//now_mino_x;
			//	now_mino_y;
			}
		}
		else {
			rotate_check = 0;
		}
	}
	
	if (rotate_check)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				now_form[i][j] = rotate_form[i][j];
				rotate_form[i][j] = 0;
				
			}
		}
		angle = (angle + r + 4) % 4;
	}

}

void init_mino() {
	nextmino[14] = -1;
	Shuffle();
	for (int i = 0; i < 7; i++) {
		nextmino[i] = shuffle[i];
	}
	Shuffle();
	for (int i = 0; i < 7; i++) {
		nextmino[i+7] = shuffle[i];
	}
}
void create_mino() {
	//now = rand() % 7;
	now = nextmino[0];
	for (int i = 0; i < 14; i++) {
		nextmino[i] = nextmino[i + 1];
	}
	if (nextmino[7] == -1) {
		Shuffle();
		for (int i = 0; i < 7; i++) {
			nextmino[7 + i] = shuffle[i];
		}
	}
	now_mino_x = 6;
	now_mino_y = 4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			now_form[i][j] = form[now][i][j];
		}
	}
	angle = 0;
}

void HOLD() {
	int tmp;
	if (hold == -1)
	{
		hold = now;
		create_mino();
	}
	else {
		tmp = now;
		now = hold;
		hold = tmp;
	}
	now_mino_x = 6;
	now_mino_y = 4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			now_form[i][j] = form[now][i][j];
		}
	}

	status = 3;
	gra_t = 0;
	holdable = 0;
}

void DeleteLine() {
	for (int i = 0; i < HIGHT; i++) {
		deletelist[i] = 1;
		for (int j = 0; j < WIDTH; j++) {
			if (mino[i + 5][j+3]!=1)
			{
				deletelist[i] = 0;
				break;
			}
		}
	}
	for (int i = 0; i < HIGHT; i++) {
		if (deletelist[i])
		{
			for (int j = 0; j < WIDTH; j++) {
				mino[i + 5][j + 3] = 0;
				for (int k = i; k > 0; k--) { //画面外上側の処理はまだ
					mino[k + 5][j + 3] = mino[k + 4][j + 3];
				}
			}
			
		}
	}
}

int SideWall(int s) {   //壁判定
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (now_form[i][j]) {
				if (mino[now_mino_y + i][now_mino_x + j +s] == 1 || mino[now_mino_y + i][now_mino_x + j + s] == 2)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int process_EST() { //ミノが設置されるか判定
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (now_form[i][j]) {
				if (mino[now_mino_y + i + 1][now_mino_x + j] == 1 || mino[now_mino_y + i + 1][now_mino_x + j] == 2)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}


void process_EST2() { //床設置判定
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (now_form[i][j]) {
				mino[now_mino_y + i][now_mino_x + j] = 1;
			}
		}
	}
}

void controll() {
	if (CheckHitKey(KEY_INPUT_A))
	{
	if(!SideWall(-1)){
		now_mino_x--;
		}
	}
	else if (CheckHitKey(KEY_INPUT_D)){
	if(!SideWall(1)){
		now_mino_x++;
		}
	}
	else if (CheckHitKey(KEY_INPUT_W)) {
		for (int i = 0; !process_EST(); i++) {
			now_mino_y++;
		}
			process_EST2();
			status = 2;
			gra_t = 0;
	}
	else if (CheckHitKey(KEY_INPUT_S)) {
		gra_t=GRA/FRAME-1;
	}
	if (CheckHitKey(KEY_INPUT_O)) {
		RotateMino(-1);
	}
	if (CheckHitKey(KEY_INPUT_P)) {
		RotateMino(1);
	}
	if (CheckHitKey(KEY_INPUT_I)) {
		if (holdable)HOLD();
	}
	

}


void process_GRAVITY() {  //落下
	gra_t++;
	if (gra_t == GRA/FRAME)
	{
		if (process_EST() == 0)
		{
			gra_t = 0;
			now_mino_y++;
		}
		else
		{
			gra_t = 0;
			process_EST2();
			status = 0;
		}
	}
}

void process_mino() {  //現在動かしているミノの状態
	for (int i = 0; i < HIGHT+2; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (mino[3 + i][3 + j] == 3)mino[3 + i][3 + j] = 0;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (now_form[i][j])
				mino[now_mino_y + i][now_mino_x + j] = 3;
		}
	}
}
void WaitingTime3() {
	wait_t++;
	for (int i = 0; i < HIGHT + 2; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (mino[3 + i][3 + j] == 3)mino[3 + i][3 + j] = 0;
		}
	}
	if (wait_t == 200 / FRAME) {
		wait_t = 0;
		status = 1;

	}
}
void WaitingTime2() {
	wait_t++;
	for (int i = 0; i < HIGHT + 2; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (mino[3 + i][3 + j] == 3)mino[3 + i][3 + j] = 0;
		}
	}
	if (wait_t == 200 / FRAME) {
		wait_t = 0;
		status = 1;
		create_mino();
		
	}
	holdable = 1;
}
void WaitingTime() {
	wait_t++;
	if (wait_t == 1000 / FRAME) {
		wait_t = 0;
		status = 1;
		create_mino();
	}
	holdable = 1;
}
void show() {

	for (int i = 0; i < HIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			/*if (mino[3 + 2 + i][3 + j]) {
				LoadGraphScreen(450 + j * (PIX+1) +1, 200+i*(PIX+1) +1, color[0], FALSE);
			}*/
			if (mino[3 + 2 + i][3 + j]) {
				LoadGraphScreen(450 + j * (PIX + 1) + 1, 200 + i * (PIX + 1) + 1, color[0], FALSE);
			}
			
		}
		//printf("\n");
	}
	for (int i = 0; i < WIDTH+1; i++) {
		DrawLine(450+i*(PIX+1), 200, 450+i*(PIX+1), 200 + HIGHT * (PIX+1), 0xFFFFFF);
	}
	for (int i = 0; i < HIGHT + 1; i++) {
		DrawLine(450 , 200 + i * (PIX + 1), 450+WIDTH*(PIX+1), 200 + i * (PIX + 1), 0xFFFFFF);
	}
	
	for (int i = 0; i < NEXT_NUM; i++) {
		DrawLine(NEXT_X, NEXT_Y + i * 60, NEXT_X, NEXT_Y + 50 + i * 60, 0xFFFF);
		DrawLine(NEXT_X + 50, NEXT_Y + i * 60, NEXT_X + 50, NEXT_Y + 50 + i * 60, 0xFFFF);
		DrawLine(NEXT_X, NEXT_Y + i * 60, NEXT_X + 50, NEXT_Y + i * 60, 0xFFFF);
		DrawLine(NEXT_X, NEXT_Y + 50 + i * 60, NEXT_X + 50, NEXT_Y + 50 + i * 60, 0xFFFF);
		if (nextmino[i] == 0 || nextmino[i] == 6)
		{
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					if (form[nextmino[i]][j][k])
						LoadGraphScreen(NEXT_X + 4 + k * (PIX + 1), NEXT_Y + 7 + i * 60 + j * (PIX + 1), color[0], FALSE);
				}
			}
		}
		else {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (form[nextmino[i]][j][k])
						LoadGraphScreen(NEXT_X + 10 + k * (PIX + 1), NEXT_Y + 13 + i * 60 + j * (PIX + 1), color[0], FALSE);
				}
			}
		}
	}

	DrawLine(HOLD_X, HOLD_Y, HOLD_X, HOLD_Y + 50, 0xFFFF);
	DrawLine(HOLD_X + 50, HOLD_Y, HOLD_X + 50, HOLD_Y + 50, 0xFFFF);
	DrawLine(HOLD_X, HOLD_Y, HOLD_X + 50, HOLD_Y, 0xFFFF);
	DrawLine(HOLD_X, HOLD_Y + 50, HOLD_X + 50, HOLD_Y + 50, 0xFFFF);
	if (hold != -1) {
		if (hold == 0 || hold == 6)
		{
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					if (form[hold][j][k])
						LoadGraphScreen(HOLD_X + 4 + k * (PIX + 1), HOLD_Y + 7+j * (PIX + 1), color[0], FALSE);
				}
			}
		}
		else {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (form[hold][j][k])
						LoadGraphScreen(HOLD_X + 10 + k * (PIX + 1), HOLD_Y + 13 + j * (PIX + 1), color[0], FALSE);
				}
			}
		}
	}
	DrawFormatString(400, 0,0xffffff, "%d",c);
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	ChangeWindowMode(TRUE);
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetGraphMode(640*2, 640*2, 32);
	SetWindowSize(640*2, 480*2);

	for (int i = 0; i < 16; i++)
	{
		mino[0][i] = 2;
		mino[1][i] = 2;
		mino[2][i] = 2;
		mino[25][i] = 2;
		mino[26][i] = 2;
		mino[27][i] = 2;
	}
	for (int i = 0; i < 28; i++)
	{
		mino[i][0] = 2;
		mino[i][1] = 2;
		mino[i][2] = 2;
		mino[i][13] = 2;
		mino[i][14] = 2;
		mino[i][15] = 2;
	}//壁判定
	status = 1;
	init_mino();
	create_mino();
	while (1) {

		
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
		if (status == 1)
			controll();
		if (status==1) {
			process_GRAVITY();
		}
		if(status==1){
			process_mino();
		}
		else if (status == 2) {
			WaitingTime2();
			DeleteLine();
		}
		else if (status == 3) {
			WaitingTime3();
		}
		else {
			WaitingTime();
			DeleteLine();
		}
		ClearDrawScreen();
		show();
		WaitTimer(FRAME);

		
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;
}