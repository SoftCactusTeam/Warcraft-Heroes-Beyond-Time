#ifndef __MODULETRANSITIONS_H__
#define __MODULETRANSITIONS_H__

#include "Module.h"

#include "Animation.h"

#include "SDL\include\SDL_rect.h"

struct UILabel;

enum fades
{
	no_fade,
	slider_fade,
	circular_fade,
};

enum fade_step
{
	none,
	fade_to_black,
	fade_from_black
};

class ModuleTransitions : public Module
{
public:
	ModuleTransitions();
	~ModuleTransitions();

	bool Start();
	bool PostUpdate();
	bool StartTransition(Module* module_off, Module* module_on, float time = 1.0f, fades kind_of_fade = slider_fade, bool cleanup_off = true, bool start_on = true);

	bool IsFading() const;
	fade_step GetStep() const;
	Uint32 GetNow() const;
	Uint32 GetTotalTime() const;
	SDL_Texture* GetTexturebyRadius(iPoint pos, uint radius, uint w, uint h);

private:
	void SliderFade();
	void CircularFade();
private:
	Module* off;
	Module* on;

	fade_step current_step = fade_step::none;
	
	Uint32 now = 0;
	Uint32 start_time = 0;
	Uint32 total_time = 0;

	SDL_Rect screen;
	SDL_Rect Slider_rect;
	fades thisFade;
	float rotationCont = 0.0f;
	bool cleanup_off = true;
	bool start_on = true;

public:
	bool loadTransition = false;
};

#endif
