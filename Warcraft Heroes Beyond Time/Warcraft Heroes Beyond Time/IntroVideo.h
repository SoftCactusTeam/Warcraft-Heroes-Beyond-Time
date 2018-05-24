#ifndef __INTRO_VIDEO_H__
#define __INTRO_VIDEO_H__

#include "Module.h"
#include "Timer.h"

struct SDL_Texture;

class IntroVideo : public Module
{
public:

	IntroVideo();

	// Destructor
	virtual ~IntroVideo();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	Timer introTime;
	bool isVideoFinished = false;
};

#endif // __INTRO_VIDEO_H__