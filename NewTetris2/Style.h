#pragma once
#include "DxLib.h"

class Style {
private:
	int minoGraph[16];
	int voidGraph;
	int pcGraph;
	int scGraph;
	int gcGraph;
	int goGraph;
	int arrowGraph;
	int commandGraph[7];
	int restartGraph;
	int titleGraph[3];
	int spinGraph[8];
	int backGraph;
	int titlebgGraph;
	int explosionGraph[10];
	int stringGraph[11];
	int numGraph[10];
	int crossGraph;
public:
	Style();
	inline int GetStyle(int idx) const { return minoGraph[idx]; }
	inline int GetVoidStyle() const { return voidGraph; }
	inline int GetPCStyle() const { return pcGraph; }
	inline int GetSCStyle() const { return scGraph; }
	inline int GetGCStyle() const { return gcGraph; }
	inline int GetGOStyle() const { return goGraph; }
	inline int GetArrowStyle() const { return arrowGraph; }
	inline int GetCommandStyle(int idx) const { return commandGraph[idx]; }
	inline int GetRestartStyle() const { return restartGraph; }
	inline int GetTitleStyle(int idx) const { return titleGraph[idx]; }
	inline int GetSpinStyle(int idx) const { return spinGraph[idx]; }
	inline int GetBackStyle() const { return backGraph; }
	inline int GetTitleBGStyle() const { return titlebgGraph; }
	inline int GetExplosionStyle(int idx) const { return explosionGraph[idx]; }
	inline int GetStringStyle(int idx) const { return stringGraph[idx]; }
	inline int GetNumStyle(int idx) const { return numGraph[idx]; }
	inline int GetCrossStyle() const { return crossGraph; }

};