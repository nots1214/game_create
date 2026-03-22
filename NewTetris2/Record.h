#pragma once

class Record {
public:
	Record();
	int scores[3];
	void SetScore(int value);
	inline int GetScore(int idx) const { return scores[idx]; }
};