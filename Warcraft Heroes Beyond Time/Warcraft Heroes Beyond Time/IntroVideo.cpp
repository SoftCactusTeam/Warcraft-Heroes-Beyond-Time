#include "p2Defs.h"
#include "Log.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "Scene.h"
#include "ModuleVideo.h"
#include "IntroVideo.h"
#include "SDL_mixer\include\SDL_mixer.h"

IntroVideo::IntroVideo() : Module()
{
	name = "scene";
}

// Destructor
IntroVideo::~IntroVideo()
{}

// Called before render is available
bool IntroVideo::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool IntroVideo::Start()
{
	App->window->SetTitle("Video Player");
	// TODO 1: Call the initialize function from the video module in the start of the scene.
	App->video->Initialize("video/sample(good).avi");
	// TODO 8: Play the music of the video using the audio module. 
	App->audio->PlayMusic("video/sample.ogg", 0.0f);

	return true;
}

// Called each loop iteration
bool IntroVideo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool IntroVideo::Update(float dt)
{
	if (!App->video->isVideoFinished)
	{
		App->video->GrabAVIFrame();

	}
	else
		Mix_PauseMusic();


	if (App->input->GetKey(SDL_SCANCODE_1) && App->video->isVideoFinished) {
		App->video->Initialize("video/sample(good).avi");
		App->audio->PlayMusic("video/sample.ogg", 0.0f);
	}

	return true;
}

// Called each loop iteration
bool IntroVideo::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool IntroVideo::CleanUp()
{
	App->video->CloseAVI();
	return true;
}
