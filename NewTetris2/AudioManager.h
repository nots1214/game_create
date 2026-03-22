#pragma once
#include "Dxlib.h"

class AudioManager {
private:
	int bgm;
	int bgm2;
	int seCLEAR;
	int seTETRIS;
	int seSET;
	int seSOFT;
	int seROTATE;
	int seMOVE;
	int seSPIN;
	int seHOLD;
	int sePC;
	int seGAMEOVER;
	int seGAMECLEAR;

public:
	AudioManager();
	inline void playBGM() { PlaySoundMem(bgm, DX_PLAYTYPE_LOOP); }
	inline void stopBGM(){ StopSoundMem(bgm); }
	inline void playBGM2(){ PlaySoundMem(bgm2, DX_PLAYTYPE_LOOP); }
	inline void stopBGM2(){ StopSoundMem(bgm2); }
	inline void playCLEAR(){ PlaySoundMem(seCLEAR, DX_PLAYTYPE_BACK); }
	inline void playTETRIS(){ PlaySoundMem(seTETRIS, DX_PLAYTYPE_BACK); }
	inline void playSET(){ PlaySoundMem(seSET, DX_PLAYTYPE_BACK); }
	inline void playSOFT(){ PlaySoundMem(seSOFT, DX_PLAYTYPE_BACK); }
	inline void playROTATE(){ PlaySoundMem(seROTATE, DX_PLAYTYPE_BACK); }
	inline void playMOVE(){ PlaySoundMem(seMOVE, DX_PLAYTYPE_BACK); }
	inline void playSPIN(){ PlaySoundMem(seSPIN, DX_PLAYTYPE_BACK); }
	inline void playHOLD(){ PlaySoundMem(seHOLD, DX_PLAYTYPE_BACK); }
	inline void playPC(){ PlaySoundMem(sePC, DX_PLAYTYPE_BACK); }
	inline void playGAMEOVER(){ PlaySoundMem(seGAMEOVER, DX_PLAYTYPE_BACK); }
	inline void playGAMECLEAR(){ PlaySoundMem(seGAMECLEAR, DX_PLAYTYPE_BACK); }
};