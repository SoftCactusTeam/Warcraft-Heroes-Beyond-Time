#include "p2Defs.h"
#include "Log.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTransitions.h"
#include "ModuleTextures.h"
#include "ModulePrinter.h"
#include "Scene.h"

#include <math.h>

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

#include "PlayerEntity.h"

ModuleTransitions::ModuleTransitions()
{
	name = "fade";
}

ModuleTransitions::~ModuleTransitions()
{}

// Load assets
bool ModuleTransitions::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	// Get screen size
	uint width = 0, height = 0, scale = 0;

	App->window->GetWindowSize(width, height);
	scale = App->window->GetScale();

	screen = { 0, 0, static_cast<int>(width * scale), static_cast<int>(height * scale) };
	//
	Slider_rect = screen;
	Slider_rect.w = 0;
	return true;
}

// Update: draw background
bool ModuleTransitions::PostUpdate()
{
	if (current_step == fade_step::none)
		return true;

	switch (thisFade)
	{
	case slider_fade:
		SliderFade();
		break;

	case circular_fade:
		CircularFade();
		break;
	}

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool ModuleTransitions::StartTransition(Module* module_off, Module* module_on, float time, fades kind_of_fade, bool cleanup_off, bool start_on)
{
	bool ret = false;

	this->cleanup_off = cleanup_off;
	this->start_on = start_on;

	if (current_step != fade_step::fade_to_black)
	{		
		thisFade = kind_of_fade;
		
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		off = module_off;
		on = module_on;

		App->scene->paused = true;
		if (kind_of_fade == fades::circular_fade)
			App->scene->player->anim = &App->scene->player->idleDown;
		ret = true;
	}

	return ret;
}

bool ModuleTransitions::IsFading() const
{
	return current_step != fade_step::none;
}

fade_step ModuleTransitions::GetStep() const 
{
	return current_step;
}

Uint32 ModuleTransitions::GetNow() const 
{
	return now;
}

Uint32 ModuleTransitions::GetTotalTime() const 
{
	return total_time;
}

void ModuleTransitions::SliderFade()
{
	now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:

		if (now >= total_time) {

			if (cleanup_off) {
				off->DeActivate();
			}
			if (start_on) {
				App->scene->actual_scene = App->scene->next_scene;
				on->Activate();
				App->scene->paused = false;
				
			}

			total_time += total_time;
			start_time = SDL_GetTicks();

			current_step = fade_from_black;
		}
		break;

	case fade_step::fade_from_black:

		normalized = 1.0f - normalized;

		if (now >= total_time) {
			current_step = fade_step::none;
		}
		break;
	}

	Slider_rect.w = normalized * screen.w;
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 255.0f);
	SDL_RenderFillRect(App->render->renderer, &Slider_rect);
}

void ModuleTransitions::CircularFade()
{
	now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);
	normalized = 1.0f - normalized;

	switch (current_step)
	{
	case fade_step::fade_to_black:
		normalized = MIN(1.0f, (float)now / (float)total_time);
		normalized = 1.0f - normalized;

		if (now >= total_time) {

			if (cleanup_off) {
				off->DeActivate();
			}
			if (start_on) {
				App->scene->actual_scene = App->scene->next_scene;
				on->Activate();
				App->scene->paused = false;
				
			}

			total_time += total_time;
			start_time = SDL_GetTicks();

			current_step = fade_from_black;
		}
		break;

	case fade_step::fade_from_black:

		normalized = MIN(1.0f, (float)now / (float)total_time);

		if (now >= total_time) {
			current_step = fade_step::none;
			App->scene->player->anim = &App->scene->player->idleUp;
		}
		break;
	}

	rotationCont += 1.0f * App->dt;

	if (rotationCont >= 0.05f)
	{
		rotationCont = 0.0f;
		if (App->scene->player->anim == &App->scene->player->idleUp)
			App->scene->player->anim = &App->scene->player->idleLeft;
		else if (App->scene->player->anim == &App->scene->player->idleLeft)
			App->scene->player->anim = &App->scene->player->idleDown;
		else if (App->scene->player->anim == &App->scene->player->idleDown)
			App->scene->player->anim = &App->scene->player->idleRight;
		else if (App->scene->player->anim == &App->scene->player->idleRight)
			App->scene->player->anim = &App->scene->player->idleUp;
	}

	SDL_Texture* toBlit = GetTexturebyRadius({ (int)App->scene->player->pos.x + 10 + App->render->camera.x,(int)App->scene->player->pos.y + 10 + App->render->camera.y}, normalized * 360, screen.w, screen.h);
	App->render->Blit(toBlit, 0, 0, nullptr, 1, 0);
	SDL_DestroyTexture(toBlit);
}

SDL_Texture* ModuleTransitions::GetTexturebyRadius(iPoint pos, uint radius, uint w, uint h)
{
	SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			float distance = sqrt(pow(x - pos.x, 2) + pow(y - pos.y, 2));

			uint32 pixel_color = distance > radius ? 0x000000FF : 0x00000000; //RRGGBBAA, in hexadecimal
			int bpp = surface->format->BytesPerPixel;

			/* Here p is the address to the pixel we want to set */
			Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

			switch (bpp) {
			case 1:
				*p = pixel_color;
				break;

			case 2:
				*(Uint16 *)p = pixel_color;
				break;

			case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
					p[0] = (pixel_color >> 16) & 0xff;
					p[1] = (pixel_color >> 8) & 0xff;
					p[2] = pixel_color & 0xff;
				}
				else {
					p[0] = pixel_color & 0xff;
					p[1] = (pixel_color >> 8) & 0xff;
					p[2] = (pixel_color >> 16) & 0xff;
				}
				break;

			case 4:
				*(Uint32 *)p = pixel_color;
				break;
			}
		}
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(App->render->renderer, surface);
	SDL_FreeSurface(surface);
	return tex;
}