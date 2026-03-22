#include "Challenge.h"
#include <ctime>

char condition[10][50] = {
	"Perform a Tetris in 5 moves!",
	"Perform a Tspin-Double in 2 moves!",
	"Perform a Sspin-Double in 1 move!",
	"Perform a Jspin-Double in 1 move!",
	"Perform a Zspin-Triple in 1 move!",
	"Perform a Lspin-Triple in 1 move!",
	"Perform a Tspin-Triple in 1 move!",
	"Perform a PerfectClear in 3 moves!",
	"Perform 15 REN in 15 moves!",
	"Perform a PerfectClear in 10 moves!"
};

Challenge::Challenge() {
	scTimer = 0;
	state = State::Playing;
	stage = 0;
	step[0] = 5;
	step[1] = 2;
	step[2] = 1;
	step[3] = 1;
	step[4] = 1;
	step[5] = 1;
	step[6] = 1;
	step[7] = 3;
	step[8] = 15;
	step[9] = 10;
	challenge.push_back({ 
		                                          {21,9},
		{22,3}                     ,{22,7}       ,{22,9},
		{23,3},{23,4},{23,5},{23,6},{23,7}       ,{23,9},{23,10},
		{24,3},{24,4},{24,5},{24,6},{24,7}       ,{24,9},{24,10},{24,11},{24,12} });
	challenge.push_back({
		{23,3},{23,4},{23,5}                            ,{23,10},{23,11},{23,12},
		{24,3},{24,4},{24,5},       {24,7}       ,{24,9},{24,10},{24,11},{24,12} });
	challenge.push_back({
		{23,3},{23,4},{23,5},{23,6},{23,7}       ,{23,10},{23,11},{23,12},
		{24,3},{24,4},{24,5},{24,6}       ,{24,9},{24,10},{24,11},{24,12}
		});
	challenge.push_back({
		{23,3},{23,4},{23,5},{23,6}       ,{23,8},{23,9},{23,10},{23,11},{23,12},
		{24,3},{24,4},{24,5},{24,6}                     ,{24,10},{24,11},{24,12}
		});
	challenge.push_back({
								    {21,7},
		{22,3},{22,4},{22,5},{22,6},{22,7},{22,8}       ,{22,10},{22,11},{22,12},
		{23,3},{23,4},{23,5},{23,6},{23,7}              ,{23,10},{23,11},{23,12},
		{24,3},{24,4},{24,5},{24,6},{24,7}       ,{24,9},{24,10},{24,11},{24,12}
		});
	challenge.push_back({
		                            {20,7},{20,8},
									{21,7},
		{22,3},{22,4},{22,5},{22,6},{22,7}       ,{22,9},{22,10},{22,11},{22,12},
		{23,3},{23,4},{23,5},{23,6},{23,7}       ,{23,9},{23,10},{23,11},{23,12},
		{24,3},{24,4},{24,5},{24,6},{24,7}              ,{24,10},{24,11},{24,12}
		});
	challenge.push_back({
									{20,7},{20,8},
									{21,7},
		{22,3},{22,4},{22,5},{22,6},{22,7}       ,{22,9},{22,10},{22,11},{22,12},
		{23,3},{23,4},{23,5},{23,6},{23,7}              ,{23,10},{23,11},{23,12},
		{24,3},{24,4},{24,5},{24,6},{24,7}       ,{24,9},{24,10},{24,11},{24,12}
		});
	challenge.push_back({
			   {18,4},{18,5},{18,6},{18,7},{18,8},{18,9},{18,10},{18,11},{18,12},
			   {19,4},{19,5},{19,6},{19,7},{19,8},{19,9},{19,10},{19,11},{19,12},
			   {20,4},{20,5},{20,6},{20,7},{20,8},{20,9},{20,10},{20,11},{20,12},
		                                          {21,9},{21,10},{21,11},{21,12},
		{22,3},{22,4},{22,5},{22,6},{22,7}       ,{22,9},{22,10},{22,11},{22,12},
		{23,3},{23,4},{23,5},{23,6},{23,7}       ,{23,9},{23,10},{23,11},{23,12},
		{24,3},{24,4},{24,5},{24,6},{24,7}       ,{24,9},{24,10},{24,11},{24,12}
		});
	challenge.push_back({
		{ 9,3},{ 9,4},{ 9,5}                            ,{ 9,10},{ 9,11},{ 9,12},
		{10,3},{10,4},{10,5}                            ,{10,10},{10,11},{10,12},
		{11,3},{11,4},{11,5}                            ,{11,10},{11,11},{11,12},
		{12,3},{12,4},{12,5}                            ,{12,10},{12,11},{12,12},
		{13,3},{13,4},{13,5}                            ,{13,10},{13,11},{13,12},
		{14,3},{14,4},{14,5}                            ,{14,10},{14,11},{14,12},
		{15,3},{15,4},{15,5}                            ,{15,10},{15,11},{15,12},
		{16,3},{16,4},{16,5}                            ,{16,10},{16,11},{16,12},
		{17,3},{17,4},{17,5}                            ,{17,10},{17,11},{17,12},
		{18,3},{18,4},{18,5}                            ,{18,10},{18,11},{18,12},
		{19,3},{19,4},{19,5}                            ,{19,10},{19,11},{19,12},
		{20,3},{20,4},{20,5}                            ,{20,10},{20,11},{20,12},
		{21,3},{21,4},{21,5}                            ,{21,10},{21,11},{21,12},
		{22,3},{22,4},{22,5}                            ,{22,10},{22,11},{22,12},
		{23,3},{23,4},{23,5},{23,6}                     ,{23,10},{23,11},{23,12},
		{24,3},{24,4},{24,5},{24,6},{24,7}              ,{24,10},{24,11},{24,12}
		});
	challenge.push_back({});
	usemino.assign(10, vector<TetroMino>(14, TetroMino::UnusedMino));
	usemino[0][0] = TetroMino::Zmino;
	usemino[0][1] = TetroMino::Jmino;
	usemino[0][2] = TetroMino::Tmino;
	usemino[0][3] = TetroMino::Omino;
	usemino[0][4] = TetroMino::Imino;
	usemino[1][0] = TetroMino::Jmino;
	usemino[1][1] = TetroMino::Tmino;
	usemino[2][0] = TetroMino::Smino;
	usemino[3][0] = TetroMino::Jmino;
	usemino[4][0] = TetroMino::Zmino;
	usemino[5][0] = TetroMino::Lmino;
	usemino[6][0] = TetroMino::Tmino;
	usemino[7][0] = TetroMino::Imino;
	usemino[7][1] = TetroMino::Imino;
	usemino[7][2] = TetroMino::Imino;
}

void Challenge::Clearing() {
	ren++;
	board->FallLines(deletelist);
	if (step[stage] == 0)
	{
		if (ClearStage()) {
			state = State::Next;
			scTimer = 50;
			audio.playPC();
		}
		else {
			state = State::GameOver;
			scTimer = 0;
		}
	}
	for (int i = 0; i < 20; i++) {
		deletelist[i] = false;
	}
}

void Challenge::Init() {
	if (stage < 8)
		current.initChallenge(usemino[stage]);
	else
		current.init_mino();
	board->Reset();
	CreateStage();
	Shading();
	ren = 0;
	spinenter = -1;
}

void Challenge::CreateStage() {
	for (int i = 0; i < challenge[stage].size(); i++)
	{
		int x = challenge[stage][i].second;
		int y = challenge[stage][i].first;
		board->SetMino(x, y, -1);
	}
}

bool Challenge::ClearStage() {
	switch(stage) {
	case 0:
		if (Num_of_Lines() == 4)
			return true;
		else
			return false;
		break;
	case 1:
		if (Num_of_Lines() == 2 && spinenter)
			return true;
		else return false;
		break;
	case 2:
		if (Num_of_Lines() == 2 && spinenter)
			return true;
		else return false;
		break;
	case 3:
		if (Num_of_Lines() == 2 && spinenter)
			return true;
		else return false;
		break;
	case 4:
		if (Num_of_Lines() == 3 && spinenter)
			return true;
		else return false;
		break;
	case 5:
		if (Num_of_Lines() == 3 && spinenter)
			return true;
		else return false;
		break;
	case 6:
		if (Num_of_Lines() == 3 && spinenter)
			return true;
		else return false;
		break;
	case 7:
		if (board->IsPerfectClear())
			return true;
		else return false;
		break;
	case 8:
		if (ren >= 15)
			return true;
		else return false;
	case 9:
		if (board->IsPerfectClear())
			return true;
		else return false;
		break;
	}
}

void Challenge::Next() {
	stage++;
	if (stage == 10)
		state = State::GameClear;
	else
	{
		state = State::Playing;
		//ShowNext();
		Init();
		if (stage < 8)
			CanHold = false;
		else CanHold = true;
		current.SetHold(TetroMino::VoidMino);
	}
}


//ホールド
void Challenge::Hold() {
	if (CanHold && stage >= 8)
	{
		audio.playHOLD();
		current.Hold();
		while (!board->CanFallShade(current))
			current.ShadeFall();
		spinenter = -1;
		gravityTimer = 0;
		CanHold = false;
		if (board->TouchUp(current))
			state = State::GameOver;
	}
}

//硬直
void Challenge::AfterLock() {
	step[stage]--;
	if (board->ClearLines(deletelist))
	{
		if ((spinenter != -1 && Num_of_Lines() >= 2) || Num_of_Lines() >= 4)
			audio.playTETRIS();
		else
			audio.playCLEAR();
		state = State::Setting;
	}
	else
	{
		if (step[stage] == 0) {
			state = State::GameOver;
			scTimer = 0;
		}
		else
			state = State::LockDelay;
		ren = 0;
	}
	waitingTimer = 0;
}

void Challenge::ShowString() {
	DrawGraph(495, 300, style.GetCommandStyle(1), TRUE);
	DrawGraph(605, 450, style.GetCommandStyle(2), TRUE);
	if (stage < 8)
		DrawGraph(585, 455, style.GetCrossStyle(), TRUE);
	if (state == State::GameOver) {
		DrawGraph(LEFTSIDE - 20, UPSIDE + PIX * 5, style.GetGOStyle(), TRUE);
	}
	if (state == State::GameClear) {
		DrawGraph(LEFTSIDE - 20, UPSIDE + PIX * 5, style.GetGCStyle(), TRUE);
	}
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 35; i++) {
			DrawGraph(LEFTSIDE - 120 + i * 10, 50 + j*10, style.GetVoidStyle(), TRUE);
		}
	}
	DrawGraph(LEFTSIDE - 90, 50 - 2, style.GetStringStyle(3), TRUE);
	DrawNums(LEFTSIDE - 50, 50, stage + 1);
	DrawFormatString(LEFTSIDE - 120, 70, 0xff8800, "%s", condition[stage]);

	if (ren > 1){
		DrawGraph(LEFTSIDE - 60, 370 - 5, style.GetStringStyle(4), TRUE);
		DrawNums(LEFTSIDE - 80, 370 - 2, ren);
	}
}

void Challenge::ShowClear() {
	while (clearframe < 20) {
		ClearDrawScreen();
		board->Show(style);
		ShowSC();
		ShowSpin();
		current.ShowHold(style);
		current.ShowNext(style);
		current.ShowShade(style);
		current.ShowMino(style);
		ShowString();
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

void Challenge::ShowSC() {
	if (scTimer > 0)
	{
		DrawGraph(LEFTSIDE - 20, UPSIDE + PIX * 5, style.GetSCStyle(), TRUE);
	}
}

//表示
void Challenge::Show() {
	ClearDrawScreen();
	board->Show(style);
	ShowSC();
	ShowSpin();
	current.ShowHold(style);
	current.ShowNext(style);
	current.ShowShade(style);
	current.ShowMino(style);
	ShowString();
	WaitTimer(FRAME);
}

//更新
void Challenge::Update(Record& record) {
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
		ShowClear();
		Clearing();
		break;
	case State::GameOver:
		WaitTimer(100);
		break;
	case State::GameClear:
		WaitTimer(100);
		break;
	case State::Next:
		Next();
		break;
	}
	if (scTimer > 0)
		scTimer--;
}

int Challenge::MainChallenge(Record& record) {

	audio.playBGM2();
	srand(time(NULL));
	Init();
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
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break; //Escで終了
			Update(record);
			Show();
			if (state == State::GameOver)
			{
				audio.stopBGM2();
				audio.playGAMEOVER();
				return false;
			}
			if (state == State::GameClear)
			{
				audio.stopBGM2();
				audio.playGAMECLEAR();
				return true;
			}
		}
	}
}
