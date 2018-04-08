#include "p2Defs.h"
#include "Log.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "FileSystem.h"

#include "Brofiler\Brofiler.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

Textures::Textures() : Module()
{
	name = "textures";
}

Textures::~Textures()
{}

bool Textures::Awake(pugi::xml_node& texturesNode)
{
	LOG("Init Image library");
	bool ret = true;
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

bool Textures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

bool Textures::CleanUp()
{
	LOG("Freeing textures and Image library");
	std::list<SDL_Texture*>::const_iterator item;

	for(item = textures.begin(); item != textures.end(); ++item)
	{
		SDL_DestroyTexture(*item);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

SDL_Texture* const Textures::Load(const char* path)
{
	BROFILER_CATEGORY("LoadTexture", Profiler::Color::Chocolate);
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load_RW(App->fs->Load(path), 1);

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

bool Textures::UnLoad(SDL_Texture* texture)
{
	std::list<SDL_Texture*>::const_iterator item;

	for (item = textures.begin(); item != textures.end(); item = ++item)
	{
		if (texture == *item)
		{
			SDL_DestroyTexture(*item);
			textures.remove(*item);
			return true;
		}
	}

	return false;
}

SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if(texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.remove(texture);
	}

	return texture;
}

void Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}
