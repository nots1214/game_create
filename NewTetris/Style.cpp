#include "Style.h"


//ミノの色
char color[7][10] = { "Imino.png","Tmino.png" ,"Jmino.png" ,"Lmino.png" ,"Zmino.png" ,"Smino.png" ,"Omino.png" };

Style::Style() {
	for (int i = 0; i < 7; i++) {
		minoGraph[i] = LoadGraph(color[i]);
	}
}

int Style::GetStyle(int idx) const { 
	return minoGraph[idx];
}