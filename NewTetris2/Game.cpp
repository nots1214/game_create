#include "Game.h"
#include <ctime>

const int scorebonus[5] = { 0, 1000,2000,4000,10000 };
const int renbonus[10] = { 0,100,200,300,500,700,900,1300,1700,2500 };

Game::Game() {
	board = new Board();
	gravityTimer = 0;
	waitingTimer = 0;
	pcTimer = 0;
	state = State::Playing;
	locktimeL = 0;
	locktimeR = 0;
	CanHold = true;
	Lhold = 0;
	Rhold = 0;
	deletelist.assign(20, false);
	score = 0;
	spinenter = -1;
	spinboard = -1;
	ren = 0;
	nextfall = GRA;
	level = 0;
	clearframe = 0;
}

void Game::DrawNums(int x,int y,int value) {
	vector<int> nums;
	int size=0;
	if (value == 0) {
		nums.push_back(0);
		size = 1;
	}
	else {
		while (value > 0) {
			nums.push_back(value % 10);
			value = value / 10;
			size++;
		}
	}
	for (int i = size - 1; i >= 0; i--) {
		DrawGraph(x + (size - 1 - i) * 11, y, style.GetNumStyle(nums[i]), TRUE);
	}
}

//落下
void Game::GRAVITYFall() {
	gravityTimer++;
	if (gravityTimer >= nextfall / FRAME)
	{
		if (board->Update(current)) {
			audio.playSOFT();
			ResetMino();
			AfterLock();
		}
		else {
			spinenter = -1;
			spinboard = -1;
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
	while (!board->CanFallShade(current))
		current.ShadeFall();
	CanHold = true;
}

void Game::Shading() {
	current.ResetShade();
	while (!board->CanFallShade(current))
		current.ShadeFall();
}

void Game::CalculateScore() {
	int clearnum = 0;
	for (int i = 0; i < 20; i++) {
		if (deletelist[i])
			clearnum++;
	}
	if (clearnum < 5)
	{
		if (spinenter == -1)
			score += scorebonus[clearnum];
		else score += scorebonus[clearnum] * 5;
	}
	if (ren >= 10) score += renbonus[9];
	else score += renbonus[ren-1];
}

void Game::PCScore() {
	score += 40000;
}

//ミノ設置した後の待ち時間
void Game::WaitingTime(int time) {
	waitingTimer++;
	if (waitingTimer >= time / FRAME) {
		waitingTimer = 0;
		SpawnMino();
		if (board->TouchUp(current))
			state = State::GameOver;
		else
			state = State::Playing;
		spinenter = -1;
		spinboard = -1;
	}
}

void Game::Setting() {
	state = State::Clearing;
	clearframe = 0;
}

void Game::Clearing() {
	ren++;
	level++;
	nextfall = max(GRA - level*5,20);
	CalculateScore();
	board->FallLines(deletelist);
	if (board->IsPerfectClear()) {
		audio.playPC();
		pcTimer = 100;
		PCScore();
	}
}

int Game::Num_of_Lines() {
	int tetriscnt = 0;
	for (int i = 0; i < 20; i++) {
		if (deletelist[i])
			tetriscnt++;
	}
	return tetriscnt;
}


//左移動
void Game::MoveLeft() {
	if (!board->TouchSideWall(-1, current)) {
		audio.playMOVE();
		current.SetX(current.GetX() - 1);
		if (Lhold > HOLDBUTTON)
			locktimeL = HOLDDELAY;
		else
			locktimeL = CONTROLLDELAY;
		spinenter = -1;
		spinboard = -1;
		Shading();
	}
}

//右移動
void Game::MoveRight() {
	if (!board->TouchSideWall(1, current)) {
		audio.playMOVE();
		current.SetX(current.GetX() + 1);
		if (Rhold > HOLDBUTTON)
			locktimeR = HOLDDELAY;
		else
			locktimeR = CONTROLLDELAY;
		spinenter = -1;
		spinboard = -1;
		Shading();
	}
}

//クイックドロップ
void Game::QuickDrop() {
	audio.playSET();
	if (current.GetY() != current.GetShadeY())
	{
		spinenter = -1;
		spinboard = -1;
	}
	current.QuickDrop();//ミノ落下
	board->process_EST2(current);//設置
	ResetMino();
	AfterLock();
	gravityTimer = 0;
}

//高速落下
void Game::FastFall() {
	if (!board->process_EST(current))
		gravityTimer = nextfall / FRAME - 1;
}

//左回転
void Game::RotateLeft() {
	if (board->CanRotate(LEFT,current)) {
		current.RotateMino(LEFT);
		spinenter = board->SpinEnter(current);
		if (spinenter != -1)audio.playSPIN();
		else audio.playROTATE();
		spinboard = -1;
		Shading();
	}
}

//右回転
void Game::RotateRight() {
	if (board->CanRotate(RIGHT,current)) {
		current.RotateMino(RIGHT);
		spinenter = board->SpinEnter(current);
		if (spinenter != -1)audio.playSPIN();
		else audio.playROTATE();
		spinboard = -1;
		Shading();
	}
}

//ホールド
void Game::Hold() {
	if (CanHold)
	{
		audio.playHOLD();
		current.Hold();
		while (!board->CanFallShade(current))
			current.ShadeFall();
		spinenter = -1;
		spinboard = -1;
		gravityTimer = 0;
		CanHold = false;
		if (board->TouchUp(current))
			state = State::GameOver;
	}
}

//硬直
void Game::AfterLock(){
	if (board->ClearLines(deletelist))
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
void Game::KeyInput(Record& record) {
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
	if (nowaction.quickdrop && !prevaction.quickdrop)
		QuickDrop();
	if(nowaction.fastfall)
		FastFall();
	if(nowaction.sethold && !prevaction.sethold)
		Hold();

	if (locktimeL > 0) locktimeL--;
	if (locktimeR > 0) locktimeR--;
}


void Game::ShowString(Record& record) {
	DrawGraph(495, 300, style.GetCommandStyle(1), TRUE);
	DrawGraph(605, 450, style.GetCommandStyle(2), TRUE);

	if (state == State::GameOver) {
		DrawGraph(LEFTSIDE - 20, UPSIDE + PIX * 5, style.GetGOStyle(), TRUE);
	}

	DrawGraph(LEFTSIDE-10, 430-5, style.GetStringStyle(1), TRUE);
	DrawNums(LEFTSIDE + 42, 426, score);
	DrawGraph(LEFTSIDE - 9, 450 - 5, style.GetStringStyle(2), TRUE);
	DrawNums(LEFTSIDE + 42, 447, level/10+1);

	if (ren > 1)
	{
		DrawGraph(LEFTSIDE - 60, 370 - 5, style.GetStringStyle(4), TRUE);
		DrawNums(LEFTSIDE -80, 370-2, ren);
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

void Game::ShowClear(Record& record) {
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
			if (deletelist[i])
			{
				for (int j = 0; j < WIDTH; j++)
				{
					if (clearframe - j >= 0 && clearframe - j < 10)
						DrawGraph(LEFTSIDE + j * (PIX + 1) - 1, UPSIDE + i * (PIX + 1) - 1, style.GetExplosionStyle(clearframe - j), TRUE);
				}
			}
		}
		clearframe++;
		WaitTimer(50);
	}
	state = State::LockDelay;
}

void Game::ShowPC() {
	if (pcTimer > 0)
	{
		DrawGraph(LEFTSIDE-20, UPSIDE + PIX * 5, style.GetPCStyle(), TRUE);
	}
}

void Game::ShowSpin() {
	if (spinenter != -1)
		DrawGraph(LEFTSIDE - 70, 400, style.GetSpinStyle(static_cast<int>(current.GetShape())),TRUE);
	if (spinboard != -1)
		DrawGraph(LEFTSIDE - 70, 300, style.GetSpinStyle(7), TRUE);
}

//表示
void Game::Show(Record& record) {
	ClearDrawScreen();
	board->Show(style);
	ShowPC();
	ShowSpin();
	current.ShowHold(style);
	current.ShowNext(style);
	current.ShowShade(style);
	current.ShowMino(style);
	ShowString(record);
	WaitTimer(FRAME);
}

//更新
void Game::Update(Record& record) {
	KeyInput(record);
	switch (state)
	{
	case State::Playing:
		GRAVITYFall();
		break;
	case State::LockDelay:
		WaitingTime(200);
		break;
	case::State::Setting:
		Setting();
		break;
	case State::Clearing:
		ShowClear(record);
		Clearing();
		break;
	case State::GameOver:
		WaitTimer(5000);
		break;
	}
	if (pcTimer > 0)
		pcTimer--;
}

void Game::MainGame(Record& record) {
	audio.playBGM();
	srand(time(NULL));
	current.init_mino();
	while (!board->CanFallShade(current))
		current.ShadeFall();

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//×ボタンで強制終了
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; //Escで終了
			
			Update(record);
			Show(record);
			if (state == State::GameOver)
			{
				record.SetScore(score);
				audio.stopBGM();
				audio.playGAMEOVER();
				return;
			}
		}
	}
	delete board;
	board = nullptr;
}
