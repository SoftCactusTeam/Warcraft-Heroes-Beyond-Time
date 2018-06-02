#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "SDL_mixer\include\SDL_mixer.h"
#include <vector>
#include "ModuleGUI.h"
#include "Application.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);


	bool Update(float dt);


	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Pause a playing music
	bool PauseMusic(float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Stop a FX
	bool HaltFX(int id = -1, int fadeseconds = 0);

	//Pause a FX
	bool PauseFX(int id = -1);

	//Resume a previously paused FX
	bool ResumeFX(int id = -1);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	//Return the channel used or -1 if errors happened
	int PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	//Set Volumes
	void setMusicVolume(uint percent);
	void setFXVolume(uint percent);

	//InitAudio

	void InitAudio();

public:

	//uint sounds here
	uint ButtonHovered = 0u;
	uint ButtonClicked = 0u;
	uint ArrowSound = 0u;
	uint Thrall_Dash_FX = 0u;
	uint Thrall_Die_FX = 0u;
	uint Thrall_Hit_FX = 0u;
	uint Thrall_Hitted_FX = 0u;
	uint Thrall_EnergyMax_FX = 0u;
	uint Thrall_AttackFX = 0u;
	uint Thrall_SkillFX = 0u;
	uint Thrall_Revive = 0u;
	uint ArcherDamaged = 0u;
	uint GuldanEncounterFX = 0u;
	uint GuldanTPFX = 0u;
	uint GuldanDieFX = 0u;
	uint GuldanFireBallFX = 0u;
	uint GuldanBalls_Ori = 0u;
	uint GuldanCircle = 0u;
	uint GuldanChargingPower = 0u;
	uint GuldanOddBallFX = 0u;
	uint GuldanThunderFX = 0u;
	uint GuldanHexBall = 0u;
	uint GuldanMeteoriteTarget = 0u;
	uint PaperItemFX = 0u;
	uint OpeningChestFX = 0u;
	uint UnlockPortalFX = 0u;
	uint Throw_BallsFX = 0u;
	uint Enemy_freezedFX = 0u;
	uint Enemy_fearedFX = 0u;
	uint GuldanSecondPhase = 0u;
	uint GuldanFireSecondPhase = 0u;
	uint GuldanSpiralFX = 0u;
	uint introVideoFX = 0u;

	//std::string BSO's
	std::string MainMenuBSO;
	std::string InGameBSO;
	std::string GuldanBSO;
	std::string WinBSO;

	//Volumes
	uint MusicVolumePercent = 20;
	uint FXVolumePercent = 50;

private:
	
	bool devicesConnected = false;

	_Mix_Music*			music;
	std::vector <Mix_Chunk*> fx;
};

#endif // __AUDIO_H__
