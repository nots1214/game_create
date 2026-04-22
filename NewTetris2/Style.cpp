#include "Style.h"


//ミノの色
char color[16][19] = { "img/Imino.png","img/Tmino.png" ,"img/Jmino.png" ,"img/Lmino.png" ,"img/Zmino.png" ,"img/Smino.png" ,"img/Omino.png","img/Cmino.png","img/IminoShade.png","img/TminoShade.png" ,"img/JminoShade.png" ,"img/LminoShade.png" ,"img/ZminoShade.png" ,"img/SminoShade.png" ,"img/OminoShade.png","img/Emino.png"};
char pc[14] = "img/PC.png";
char sc[19] = "img/stageclear.png";
char gc[19] = "img/gameclear.png";
char go[19] = "img/gameover.png";
char arrow[14] = "img/arrow.png";
char command[8][19] = { "img/command1.png","img/command2.png", "img/command3.png","img/command4.png","img/command5.png","img/command6.png","img/command7.png","img/command8.png" };
char restart[19] = "img/restart.png";
char title[3][18] = { "img/newtitle.png", "img/Menu2.png", "img/newmode.png"};
char spin[8][14] = { "img/Ispin.png","img/Tspin.png","img/Jspin.png","img/Lspin.png","img/Zspin.png","img/Sspin.png","img/Ospin.png","img/Bspin.png" };
char voidmino[14] = "img/void.png";
char back[14] = "img/back.png";
char titlebg[16] = "img/titlebg.png";
char explosion[22] = "img/explosion10.png";
char strings[11][24] = { "img/scoreboard.png","img/score.png","img/level.png","img/stage.png","img/ren.png","img/next.png","img/hold.png","img/1st.png","img/2nd.png","img/3rd.png","img/bestscore.png" };
char nums[16] = "img/number.png";
char cross[14] = "img/cross.png";
Style::Style() {
	for (int i = 0; i < 16; i++) {
		minoGraph[i] = LoadGraph(color[i]);
	}
	pcGraph = LoadGraph(pc);
	scGraph = LoadGraph(sc);
	gcGraph = LoadGraph(gc);
	goGraph = LoadGraph(go);
	arrowGraph = LoadGraph(arrow);
	for (int i = 0; i < 8; i++)
		commandGraph[i] = LoadGraph(command[i]);
	restartGraph = LoadGraph(restart);
	for (int i = 0; i < 3; i++) {
		titleGraph[i] = LoadGraph(title[i]);
	}
	for (int i = 0; i < 8; i++) {
		spinGraph[i] = LoadGraph(spin[i]);
	}
	voidGraph = LoadGraph(voidmino);
	backGraph = LoadGraph(back);
	titlebgGraph = LoadGraph(titlebg);
	LoadDivGraph(explosion,10,10,1,15,15,explosionGraph);
	for (int i = 0; i < 11; i++) {
		stringGraph[i] = LoadGraph(strings[i]);
	}
	LoadDivGraph(nums, 10, 10, 1, 11, 12, numGraph);
	crossGraph = LoadGraph(cross);
}
