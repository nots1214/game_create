#include "Game2.h"
#include <ctime>

const int scorebonus[6] = { 0, 1000,2000,4000,10000,15000 };
const int renbonus[10] = { 0,100,200,300,500,700,900,1300,1700,2500 };

Game2::Game2(int centerx) : Game(centerx) {
	delete board;
	board = new Board2();
	deletehorizon.assign(board->GetWIDTH(), false);
	deletevertical.assign(board->GetHEIGHT(), false);
	for (int i = 0; i < 4; i++) {
		mem.push_back({});
		for (int j = 0; j < 4; j++) 
			mem[i].push_back(false);
	}
}

//ミノ初期化
void Game2::ResetMino() {
	for (int i = 0; i < 4; i++) { for (int j = 0; j < 4; j++) mem[i][j] = current.GetMino(j, i); }
	current.ResetMino();
}

//スコア計算
void Game2::CalculateScore() {
	int clearnum = 0;
	for (int i = 0; i < board->GetWIDTH(); i++) {
		if (deletehorizon[i])
			clearnum++;
		if (deletevertical[i])
			clearnum++;
	}
	if (clearnum < 6)
	{
		if (spinenter == -1 && spinboard == -1)
			score += scorebonus[clearnum];
		else if(spinboard != -1)
			score += scorebonus[clearnum] * 7;
		else
			score += scorebonus[clearnum] * 5;
	}
	if (ren >= 10) score += renbonus[9];
	else score += renbonus[ren - 1];
}

//ライン消去の処理
void Game2::Clearing() {
	ren++;
	level++;
	nextfall = max(GRA - level * 5, 20);
	CalculateScore();
	board->FallLines(deletehorizon,deletevertical);
	if (board->IsPerfectClear()) {
		audio.playPC();
		pcTimer = 100;
		PCScore();
	}
}

//同時消し
int Game2::Num_of_Lines() {
	int tetriscnt = 0;
	for (int i = 0; i < board->GetWIDTH(); i++) {
		if (deletehorizon[i])
			tetriscnt++;
		if (deletevertical[i])
			tetriscnt++;
	}
	return tetriscnt;
}

//盤面回転
void Game2::RotateBoardLeft(Record& record) {
	board->CreateRotateBoard(LEFT);
	if (board->CanRotateBoard(current)) {
		ShowRotate(LEFT,record);
		board->RotateBoard();
		spinboard = board->IsBoardSpin(current);
		if (spinenter != -1)audio.playSPIN();
		else audio.playROTATE();

		Shading();
	}
	board->ResetRotateBoard();
}

void Game2::RotateBoardRight(Record& record) {
	board->CreateRotateBoard(RIGHT);
	if (board->CanRotateBoard(current)) {
		ShowRotate(RIGHT,record);
		board->RotateBoard();
		spinboard = board->IsBoardSpin(current);
		if (spinenter != -1)audio.playSPIN();
		else audio.playROTATE();

		Shading();
	}
	board->ResetRotateBoard();
}

void Game2::RotateBoardHalf(Record& record) {
	board->CreateRotateBoard(0);
	if (board->CanRotateBoard(current)) {
		ShowRotate(0,record);
		board->RotateBoard();
		spinboard = board->IsBoardSpin(current);
		if (spinenter != -1)audio.playSPIN();
		else audio.playROTATE();
		Shading();
	}
	board->ResetRotateBoard();
}

//硬直
void Game2::AfterLock() {
	if (board->ClearLines(deletehorizon,deletevertical,spinboard,current,mem))
	{
		if ((spinenter != -1 && Num_of_Lines() >= 2) || Num_of_Lines() == 4)
			audio.playTETRIS();
		else
			audio.playCLEAR();
		state = State::Setting;
	}
	else
	{
		state = State::LockDelay;
		ren = 0;
	}
	waitingTimer = 0;
}

//キー入力
void Game2::KeyInput(Record& record) {
	prevaction = nowaction;
	nowaction = input.GetCommand();
	prevcommand = nowcommand;
	nowcommand = input.GetMenuCommand();

	Holding();

	if (state == State::Playing) {
		if (nowaction.rotateL && !prevaction.rotateL)
			RotateLeft();
		if (nowaction.rotateR && !prevaction.rotateR)
			RotateRight();
		if (nowaction.rotBoardL && !prevaction.rotBoardL)
			RotateBoardLeft(record);
		if (nowaction.rotBoardR && !prevaction.rotBoardR)
			RotateBoardRight(record);
		if (nowaction.rotBoardH && !prevaction.rotBoardH)
			RotateBoardHalf(record);
		if (nowaction.moveleft && locktimeL <= 0)
			MoveLeft();
		if (nowaction.moveright && locktimeR <= 0)
			MoveRight();
		if (nowaction.quickdrop && !prevaction.quickdrop)
			QuickDrop();
		if (nowaction.fastfall)
			FastFall();
		if (nowaction.sethold && !prevaction.sethold)
			Hold();
		if (nowaction.pausing && !prevaction.pausing)
			Pausing();

		if (locktimeL > 0) locktimeL--;
		if (locktimeR > 0) locktimeR--;
	}
	else if (state == State::Pausing)
	{
		if (nowaction.pausing && !prevaction.pausing)
			state = State::Playing;

		if (!prevcommand.up && nowcommand.up) pauseselect = (pauseselect + 1) % 2;
		if (!prevcommand.down && nowcommand.down) pauseselect = (pauseselect + 1) % 2;
		if (!prevcommand.select && nowcommand.select) {
			if (pauseselect == 0)
			{
				state = State::Playing;
			}
			if (pauseselect == 1) {
				state = State::GameOver;
				pauseselect = 0;
			}
		}
		if ((!prevcommand.up && nowcommand.up) || (!prevcommand.down && nowcommand.down))
			ShowPause();
	}
}


void Game2::ShowString(Record& record) {
	DrawGraph(480, 300, style.GetCommandStyle(1), TRUE);
	DrawGraph(605, 450, style.GetCommandStyle(2), TRUE);
	DrawGraph(430 + 11,170,style.GetCommandStyle(5), TRUE);
	DrawGraph(338 + 11, 170, style.GetCommandStyle(6), TRUE);
	DrawGraph(696, 450, style.GetCommandStyle(7), TRUE);

	if (state == State::GameOver) {
		DrawGraph(LEFTSIDE - 9, UPSIDE + PIX * 5, style.GetGOStyle(), TRUE);
	}

	DrawGraph(LEFTSIDE - 10, 430 - 94, style.GetStringStyle(1), TRUE);
	DrawNums(LEFTSIDE + 42, 426-89, score);
	DrawGraph(LEFTSIDE - 9, 450 +6 -  100, style.GetStringStyle(2), TRUE);
	DrawNums(LEFTSIDE + 42, 447-89, level / 10 + 1);

	if (ren > 1)
	{
		DrawGraph(LEFTSIDE - 60, 370 - 5, style.GetStringStyle(4), TRUE);
		DrawNums(LEFTSIDE - 80, 370 - 2, ren);
	}
	DrawGraph(43, 355, style.GetStringStyle(0), TRUE);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 12; j++)
			DrawGraph(86 + j * PIX, 438 + i * PIX, style.GetVoidStyle(), TRUE);
	}
	DrawGraph(90, 440, style.GetStringStyle(7), TRUE);
	DrawGraph(90, 460, style.GetStringStyle(8), TRUE);
	DrawGraph(90, 480, style.GetStringStyle(9), TRUE);
	DrawNums(123, 441, record.GetScore(0));
	DrawNums(123, 461, record.GetScore(1));
	DrawNums(123, 481, record.GetScore(2));
	DrawLine(85, 437, 85, 498, 0xffffff, TRUE);
	DrawLine(85, 437, 206, 437, 0xffffff, TRUE);
	DrawLine(206, 437, 206, 498, 0xffffff, TRUE);
	DrawLine(85, 498, 206, 498, 0xffffff, TRUE);
}

void Game2::ShowRotate(int r,Record& record) {
	for (int i = 1; i < 10; i++) {
		ClearDrawScreen();
		board->ShowRotate(r, i,style);
		ShowPC();
		ShowSpin();
		current.ShowHold(style);
		current.ShowNext(style);
		current.ShowShade(style);
		current.ShowMino(style);
		ShowString(record);
		WaitTimer(50);
	}
}

void Game2::ShowClear(Record& record) {
	while (clearframe < 20) {
		ClearDrawScreen();
		board->Show(style);
		ShowPC();
		ShowSpin();
		current.ShowHold(style);
		current.ShowNext(style);
		current.ShowShade(style);
		current.ShowMino(style);
		ShowString(record);
		for (int i = 0; i < board->GetHEIGHT(); i++) {
			if (deletehorizon[i])
			{
				for (int j = 0; j < board->GetWIDTH(); j++)
				{
					if (clearframe - j >= 0 && clearframe - j < 10)
						DrawGraph(LEFTSIDE + j * (PIX + 1) - 1, UPSIDE + i * (PIX + 1) - 1, style.GetExplosionStyle(clearframe - j), TRUE);
				}
			}
		}
		for (int i = 0; i < board->GetWIDTH(); i++) {
			if (deletevertical[i])
			{
				for (int j = 0; j < board->GetHEIGHT(); j++)
				{
					if (clearframe - j >= 0 && clearframe - j < 10)
						DrawGraph(LEFTSIDE + i * (PIX + 1) - 1, UPSIDE + j * (PIX + 1) - 1, style.GetExplosionStyle(clearframe - j), TRUE);
				}
			}
		}
		clearframe++;
		WaitTimer(50);
	}
	state = State::LockDelay;
}