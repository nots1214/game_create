#pragma once
#include "Game.h"

class Challenge : public Game {
private:
	int scTimer;
	int stage;
	vector<vector<pair<int,int>>> challenge;
	int step[10];
	vector<vector<TetroMino>> usemino;

	void Clearing() override;
	void Next();
	bool ClearStage();
	void CreateStage();
	void Init();
	void Hold() override;
	void AfterLock() override;

	void ShowString();
	void ShowClear();
	void ShowSC();
public:
	Challenge();
	int MainChallenge(Record& record);
	void Show();
	void Update(Record& record);

};