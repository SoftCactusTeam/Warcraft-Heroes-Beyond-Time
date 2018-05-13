#include "p2Defs.h"
#include "Log.h"
#include "ModuleAudio.h"
#include <list>
#include "Application.h"
#include "FileSystem.h"
#include "Scene.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

Audio::Audio() : Module()
{
	music = NULL;
	name = "audio";
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake(pugi::xml_node& audioNode)
{
	LOG("Loading Audio Mixer");
	
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	if (SDL_GetNumAudioDevices(0) > 0)
		devicesConnected = true;

	
	if (devicesConnected)
	{
		InitAudio();
	}
	
	return true;
}


void Audio::InitAudio()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	Mix_AllocateChannels(16);

	//---------------------------------------Load fx-------------------------------
	ButtonClicked = LoadFx("audio/fx/gui/ButtonClick.ogg");
	ButtonHovered = LoadFx("audio/fx/gui/ButtonMouseHover.ogg");
	ArrowSound = LoadFx("audio/fx/enemies/Arrow_shoot.ogg");
	Thrall_Dash_FX = LoadFx("audio/fx/thrall/T_dash_fx.ogg");
	Thrall_Die_FX = LoadFx("audio/fx/thrall/T_die_fx.ogg");
	Thrall_EnergyMax_FX = LoadFx("audio/fx/thrall/T_maxenergy_fx.ogg");
	Thrall_Hitted_FX = LoadFx("audio/fx/thrall/T_hitted_fx.ogg");
	Thrall_Hit_FX = LoadFx("audio/fx/thrall/T_hit_fx.ogg");
	Thrall_AttackFX = LoadFx("audio/fx/thrall/T_attack_fx.ogg");
	Thrall_SkillFX = LoadFx("audio/fx/thrall/T_skill_fx.ogg");
	ArcherDamaged = LoadFx("audio/fx/enemies/ArcherDeath.ogg");
	GuldanEncounterFX = LoadFx("audio/fx/guldan/guldan_first_encounter_fx.ogg");
	GuldanTPFX = LoadFx("audio/fx/guldan/guldan_tp_fx.ogg");
	GuldanDieFX = LoadFx("audio/fx/guldan/guldan_die_fx.ogg");
	GuldanFireBallFX = LoadFx("audio/fx/guldan/guldan_balls_shooting.ogg");
	PaperItemFX = LoadFx("audio/fx/others/item_wcpaper_fx.ogg");
	OpeningChestFX = LoadFx("audio/fx/others/open_chest_fx.ogg");
	UnlockPortalFX = LoadFx("audio/fx/others/portal_unlocked_fx.ogg");
	GuldanBalls_Ori = LoadFx("audio/fx/guldan/guldan_fireball_fx.ogg");
	Throw_BallsFX = LoadFx("audio/fx/guldan/throw_balls_fx.ogg");
	Enemy_freezedFX = LoadFx("audio/fx/enemies/freezed_fx.ogg");
	Enemy_fearedFX = LoadFx("audio/fx/enemies/fear_fx.ogg");
	GuldanCircle = LoadFx("audio/fx/guldan/circleballs_explosion_fx.ogg");
	GuldanOddBallFX = LoadFx("audio/fx/guldan/espiral_fx.ogg");
	GuldanChargingPower = LoadFx("audio/fx/guldan/charging_power_fx.ogg");
	GuldanThunderFX = LoadFx("audio/fx/guldan/G_thunder.ogg");
	//---------------------------------------Load Music----------------------------
	MainMenuBSO = "audio/BSO's/Warcraft HBT - Main Menu.ogg";
	InGameBSO = "audio/BSO's/Warcraft HBT - Level.ogg";
	GuldanBSO = "audio/BSO's/Warcraft HBT - Guldan.ogg";
	WinBSO = "audio/BSO's/Warcraft HBT - Win.ogg";
	//---------------------------------------SetVolumes----------------------------

	Mix_Volume(-1, (MIX_MAX_VOLUME * FXVolumePercent) / 100);
	Mix_VolumeMusic((MIX_MAX_VOLUME * MusicVolumePercent) / 100);
}


bool Audio::Update(float dt)
{
	if (devicesConnected == false && SDL_GetNumAudioDevices(0) > 0)
	{
		devicesConnected = true;
		InitAudio();
	}

	return true;
}



// Called before quitting
bool Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for (uint i = 0; i < fx.size(); ++i)
		Mix_FreeChunk(fx[i]);
	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!devicesConnected)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS_RW(App->fs->Load(path), 1);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

bool Audio::PauseMusic(float fade_time)
{
	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}
	}
	return Mix_PlayingMusic() == 0;
}

bool Audio::HaltFX(int id, int fadeseconds)
{
	if (!devicesConnected)
		return false;

	if (id == -1)
	{
		Mix_HaltChannel(-1);
	}
	else
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_HaltChannel(i);
		}

	return true;
}

bool Audio::PauseFX(int id)
{
	if (!devicesConnected)
		return false;

	if (id == -1)
		Mix_Pause(-1);
	else
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_Pause(i);
		}
	return true;
}

bool Audio::ResumeFX(int id)
{
	if (!devicesConnected)
		return false;
	
	if (id == -1)
	{
		Mix_Resume(-1);
	}
	else
	{
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_Resume(i);
			break;
		}
	}
}

// Load WAV
unsigned int Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(App->fs->Load(path), 1);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play a previously loaded WAV
//Return the channel used or -1 if errors happened
int Audio::PlayFx(unsigned int id, int repeat, int channel)
{
	if (!devicesConnected)
		return -1;

	if (id > 0 && id <= fx.size())
	{
		return Mix_PlayChannel(channel, fx[id - 1], repeat);
	}

	return -1;
}

void Audio::setMusicVolume(uint percent)
{
	if (!devicesConnected)
		return;

	MusicVolumePercent = percent;
	Mix_VolumeMusic((MIX_MAX_VOLUME * MusicVolumePercent) / 100);
}

void Audio::setFXVolume(uint percent)
{
	if (!devicesConnected)
		return;

	FXVolumePercent = percent;
	Mix_Volume(-1, (MIX_MAX_VOLUME * FXVolumePercent) / 100);
}