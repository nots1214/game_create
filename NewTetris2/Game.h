#pragma once
#include "Board.h"
#include "Mino.h"
#include "Input.h"
#include "Style.h"
#include "Record.h"
#include "AudioManager.h"

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
	Setting,
	Clearing,
	GameOver,
	GameClear,
	Next
};

class Game {
private:

protected:
	Board* board;
	Mino current;
	Input input;
	Style style;
	AudioManager audio;

	InputState nowaction;
	InputState prevaction;

	int gravityTimer;
	int waitingTimer;
	int pcTimer;
	State state;
	int locktimeL;
	int locktimeR;
	int Lhold;
	int Rhold;
	bool CanHold;
	vector<bool> deletelist;
	int score;
	int spinenter;
	int spinboard;
	int ren;
	int nextfall;
	int level;
	int clearframe;

	virtual void GRAVITYFall();
	virtual void ResetMino();
	virtual void SpawnMino();
	virtual void Shading();
	virtual void WaitingTime(int time);
	virtual void KeyInput(Record& record);
	void Holding();
	virtual void Setting();
	virtual void Clearing();
	virtual void CalculateScore();
	virtual void PCScore();
	virtual int Num_of_Lines();

	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void QuickDrop();
	virtual void FastFall();
	virtual void RotateLeft();
	virtual void RotateRight();
	virtual void Hold();
	virtual void AfterLock();

	virtual void ShowString(Record& record);
	virtual void ShowClear(Record& record);
	void ShowPC();
	virtual void ShowSpin();
	void DrawNums(int x, int y, int value);
public:
	Game();
	inline int GetScore() { return score; }
	virtual void MainGame(Record& record);
	virtual void Show(Record& record);
	virtual void Update(Record& record);

};