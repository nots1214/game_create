#pragma once
#include "Board.h"
#include "Mino.h"
#include "Input.h"
#include "Style.h"

#define CONTROLLDELAY 4
#define HOLDDELAY 1
#define FRAME 50
#define GRA 1000
#define LEFT -1
#define RIGHT 1
#define HOLDMAX 100
#define HOLDBUTTON 5



enum class State {
	Playing,
	LockDelay,
	Clearing
};

class Game {
private:
	Board board;
	Mino current;
	Input input;
	Style style;

	InputState nowaction;
	InputState prevaction;

	int gravityTimer;
	int waitingTimer;
	State state;
	int locktimeL;
	int locktimeR;
	int Lhold;
	int Rhold;
	bool CanHold;

	void process_GRAVITY();
	void ResetMino();
	void SpawnMino();
	void WaitingTime(int time);
	void KeyInput();
	void Holding();

	void MoveLeft();
	void MoveRight();
	void QuickDrop();
	void FastFall();
	void RotateLeft();
	void RotateRight();
	void Hold();
	void AfterLock();
public:
	Game();
	void MainGame();
	void Show();
	void Update();

};