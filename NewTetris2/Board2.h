#pragma once

#include "Board.h"

class Mino;

class Board2 : public Board {
private:

protected:
	vector<vector<int>> rotate_board;
	bool isMino(int x, int y) override;
public:
	Board2();
	int IsBoardSpin(Mino& current) override;
	void RotateBoard() override;
	bool CanRotateBoard(Mino& current) override;
	void CreateRotateBoard(int r) override;
	void ResetRotateBoard() override;
	bool ClearLines(vector<bool>& deletehorizon, vector<bool>& deletevertical,int spinboard,Mino& current, vector<vector<bool>>& mem) override;
	void FallLines(vector<bool>& deletehorizon, vector<bool>& deletevertical) override;
	virtual int GetHEIGHT() override;
	void ShowRotate(int r, int k, Style& style) override;
};
