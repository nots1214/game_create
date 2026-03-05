#include "Game.h"
#include <ctime>

Game::Game() {
	gravityTimer = 0;
	waitingTimer = 0;
	state = State::Playing;
	locktimeL = 0;
	locktimeR = 0;
	CanHold = true;
	Lhold = 0;
	Rhold = 0;
}

//落下
void Game::process_GRAVITY() {
	gravityTimer++;
	if (gravityTimer >= GRA / FRAME)
	{
		if (board.Update(current)) {
			ResetMino();
			AfterLock();
		}
		gravityTimer = 0;
	}
}

//ミノ初期化
void Game::ResetMino() {
	current.ResetMino();
}

//ミノ出現
void Game::SpawnMino() {
	current.NewMino();
	CanHold = true;
}

//ライン消去したあとの待ち時間
void Game::WaitingTime(int time) {
	waitingTimer++;
	if (waitingTimer >= time / FRAME) {
		waitingTimer = 0;
		SpawnMino();
		state = State::Playing;
	}
}



//左移動
void Game::MoveLeft() {
	if (!board.TouchSideWall(-1, current)) {
		current.SetX(current.GetX() - 1);
		if (Lhold > HOLDBUTTON)
			locktimeL = HOLDDELAY;
		else
			locktimeL = CONTROLLDELAY;
	}
}

//右移動
void Game::MoveRight() {
	if (!board.TouchSideWall(1, current)) {
		current.SetX(current.GetX() + 1);
		if (Rhold > HOLDBUTTON)
			locktimeR = HOLDDELAY;
		else
			locktimeR = CONTROLLDELAY;
	}
}

//クイックドロップ
void Game::QuickDrop() {
	while (!board.process_EST(current)) {
		current.Fall();
	}
	board.process_EST2(current);
	ResetMino();
	AfterLock();
	gravityTimer = 0;
}

//高速落下
void Game::FastFall() {
	if (!board.process_EST(current))
		gravityTimer = GRA / FRAME - 1;
}

//左回転
void Game::RotateLeft() {
	if (board.CanRotate(LEFT,current)) {
		current.RotateMino(LEFT);
	}
}

//右回転
void Game::RotateRight() {
	if (board.CanRotate(RIGHT,current)) {
		current.RotateMino(RIGHT);
	}
}

//ホールド
void Game::Hold() {
	if (CanHold)
	{
		current.Hold();
		gravityTimer = 0;
		CanHold = false;
	}
}

//硬直
void Game::AfterLock(){
	if (board.ClearLines())
		state = State::Clearing;
	else
		state = State::LockDelay;

	waitingTimer = 0;
}

//長押し判定
void Game::Holding() {
	if (nowaction.moveleft)
	{
		if (Lhold < HOLDMAX)
		{
			Lhold++;
		}
	}
	else
		Lhold = 0;

	if (nowaction.moveright)
	{
		if (Rhold < HOLDMAX)
		{
			Rhold++;
		}
	}
	else
			Rhold = 0;
}

//キー入力
void Game::KeyInput() {
	prevaction = nowaction;
	nowaction = input.GetCommand();

	Holding();

	if (state != State::Playing) return;


	if (nowaction.rotateL && !prevaction.rotateL)
		RotateLeft();
	if (nowaction.rotateR && !prevaction.rotateR)
		RotateRight();
	if (nowaction.moveleft && locktimeL <= 0)
		MoveLeft();
	if (nowaction.moveright && locktimeR <= 0)
		MoveRight();
	if(nowaction.quickdrop && !prevaction.quickdrop)
		QuickDrop();
	if(nowaction.fastfall)
		FastFall();
	if(nowaction.sethold && !prevaction.sethold)
		Hold();

	if (locktimeL > 0) locktimeL--;
	if (locktimeR > 0) locktimeR--;
}

//表示
void Game::Show() {
	ClearDrawScreen();
	board.Show(style);
	current.ShowHold(style);
	current.ShowNext(style);
	current.ShowMino(style);
	WaitTimer(FRAME);
}

//更新
void Game::Update() {
	KeyInput();
	switch (state)
	{
	case State::Playing:
		process_GRAVITY();
		break;
	case State::LockDelay:
		WaitingTime(200);
		break;
	case State::Clearing:
		WaitingTime(1000);
		break;
	}
}

void Game::MainGame() {

	srand(time(NULL));
	current.init_mino();

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//×ボタンで強制終了
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break; //Escで終了
			Update();
			Show();
		}
	}
}
