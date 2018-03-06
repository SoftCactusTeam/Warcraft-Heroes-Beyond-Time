#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "SDL_mixer\include\SDL_mixer.h"
#include <vector>
#include "ModuleGUI.h"
#include "App.h"

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

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	//bool UI_Do(const GUIElem* elem, Events* event);

public:

	//uint sounds here

private:
	
	_Mix_Music*			music;
	std::vector <Mix_Chunk*> fx;
};

#endif // __AUDIO_H__