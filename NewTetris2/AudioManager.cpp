#include "AudioManager.h"


AudioManager::AudioManager() {
	bgm = LoadSoundMem("music/bgm.wav");
	bgm2 = LoadSoundMem("music/bgm2.wav");
	seCLEAR = LoadSoundMem("music/explosion.wav");
	seTETRIS = LoadSoundMem("music/cleartetris2.wav");
	seSET = LoadSoundMem("music/rotate2.wav");
	seSOFT = LoadSoundMem("music/soft.wav");
	seROTATE = LoadSoundMem("music/move.wav");
	seMOVE = LoadSoundMem("music/move2.wav");
	seSPIN = LoadSoundMem("music/spin.wav");
	seHOLD = LoadSoundMem("music/hold.wav");
	sePC = LoadSoundMem("music/pc.wav");
	seGAMEOVER = LoadSoundMem("music/gameover.wav");
	seGAMECLEAR = LoadSoundMem("music/gameclear.wav");
	ChangeVolumeSoundMem(60, bgm);
	ChangeVolumeSoundMem(60, bgm2);
	ChangeVolumeSoundMem(100, seMOVE);
}
