#pragma once
#include "Board2.h"

#include "Game.h"

class Game2 : public Game{
private:
	vector<bool> deletehorizon;
	vector<bool> deletevertical;
	vector<vector<bool>> mem;

	void ResetMino() override;
	void KeyInput(Record& record) override;
	void Clearing() override;
	void CalculateScore() override;
	int Num_of_Lines() override;

	void RotateBoardLeft(Record& record);
	void RotateBoardRight(Record& record);
	void RotateBoardHalf(Record& record);
	void AfterLock() override;

	void ShowString(Record& record) override;
	void ShowClear(Record& record) override;
	void ShowRotate(int r, Record& record);
public:
	Game2(int centerx);
};