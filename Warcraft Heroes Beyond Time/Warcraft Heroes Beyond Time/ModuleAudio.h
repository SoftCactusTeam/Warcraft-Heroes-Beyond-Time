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

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Pause a playing music
	bool PauseMusic(float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Pause a channel
	bool PauseFX(int id, int fadeseconds = 0);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	//Return the channel used or -1 if errors happened
	int PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	//Set Volumes
	void setMusicVolume(uint percent);
	void setFXVolume(uint percent);


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
	uint ArcherDeath = 0u;
	uint GuldanEncounterFX = 0u;
	uint GuldanTPFX = 0u;
	uint GuldanDieFX = 0u;
	uint GuldanFireBallFX = 0u;
	uint GuldanBalls_Ori = 0u;
	uint PaperItemFX = 0u;
	uint OpeningChestFX = 0u;
	uint UnlockPortalFX = 0u;
	uint Throw_BallsFX = 0u;

	//std::string BSO's
	std::string MainMenuBSO;
	std::string InGameBSO;
	std::string GuldanBSO;
	std::string WinBSO;

	//Volumes
	uint MusicVolumePercent = 10;
	uint FXVolumePercent = 10;

private:
	
	_Mix_Music*			music;
	std::vector <Mix_Chunk*> fx;
};

#endif // __AUDIO_H__
