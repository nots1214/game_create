#include "Record.h"



Record::Record() {
	for (int i = 0; i < 3; i++) {
		scores[i] = 0;
	}
}

void Record::SetScore(int value) {
	int tmp;
	if (scores[2] < value)
	{
		scores[2] = value;
		if (scores[1] < scores[2]) {
			tmp = scores[2];
			scores[2] = scores[1];
			scores[1] = tmp;
			if (scores[0] < scores[1]) {
				tmp = scores[1];
				scores[1] = scores[0];
				scores[0] = tmp;
			}
		}
	}
}

//int Record::GetScore(int idx) const {
//	return scores[idx];
//}