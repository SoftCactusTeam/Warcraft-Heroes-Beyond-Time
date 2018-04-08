#include "p2Defs.h"
#include "Log.h"

#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleMapGenerator.h"

#define VSYNC true

Render::Render() : Module()
{
	name = "renderer";

	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

Render::~Render() {}

bool Render::Awake(pugi::xml_node& renderNode)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	vsync = renderNode.child("vsync").attribute("active").as_bool();
	if(vsync == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);


	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		{
			ret = false;
		}
		else
		{
			int w, h;
			camera.w = (dm.w * 640) / 1920/*App->window->screen_surface->w*/;
			camera.h = (dm.h * 360) / 1080/*App->window->screen_surface->h*/;
			camera.x = 0;
			camera.y = 0;
			fcamerax = 0;
			fcameray = 0;

			uint width = 0, height = 0;
			App->window->GetWindowSize(width, height);

			ret = SDL_RenderSetLogicalSize(renderer, camera.w, camera.h) == 0;

			SetBackgroundColor({ 0, 205, 193, 0 });
		}
	}

	return ret;
}

bool Render::Start()
{
	LOG("render start");
	SDL_RenderGetViewport(renderer, &viewport);

	return true;
}

bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);

	return true;
}

bool Render::Update(float dt)
{
	CheckCameraLimits();

	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

bool Render::Blit(const SDL_Texture* texture, int x, int y, const SDL_Rect* section, float scale, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x /** scale*/;
	rect.y = (int)(camera.y * speed) + y /** scale*/;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, (SDL_Texture*)texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void Render::CheckCameraLimits()
{
	uint mapwidth, mapheight;
	uint tilesize = App->map->getTileSize();
	App->map->getSize(mapwidth, mapheight);

	if (-fcamerax < 0)
		fcamerax = 0;

	else if (-fcamerax + camera.w >(mapwidth * (tilesize - 2) + (tilesize - 2)))
	{
		fcamerax = -1 * (int)(mapwidth * (tilesize - 2) + (tilesize - 2) - camera.w);
	}

	if (-fcameray < 0)
		fcameray = 0;

	else if (-fcameray + camera.h >(mapheight * (tilesize-2) + tilesize-2))
	{
		fcameray = -1 * (int)(mapheight * (tilesize - 2) + (tilesize - 2) - camera.h);
	}
	
	camera.x = (int)fcamerax;
	camera.y = (int)fcameray;

	
}
