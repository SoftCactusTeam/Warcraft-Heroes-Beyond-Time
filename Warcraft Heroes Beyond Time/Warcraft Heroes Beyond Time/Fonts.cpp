#include "p2Defs.h"
#include "Log.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Fonts.h"
#include "FileSystem.h"

#include "SDL\include\SDL.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

Fonts::Fonts() : Module()
{
	name = "fonts";
}

// Destructor
Fonts::~Fonts()
{}

// Called before render is available
bool Fonts::Awake(pugi::xml_node& fontsNode)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if(TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
	//	const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
	//	int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
	//	//default = Load(path, "OpenSans-Regular", size);

	//	//Load all Fonts

		Load("fonts/arial.ttf", "Arial30", 30);
		Load("fonts/arial.ttf", "Arial40", 40);
		Load("fonts/arial.ttf", "Arial65", 65);
		default = Load("fonts/arial.ttf", "Arial80", 80);
		
		Load("fonts/LifeCraft_Font.ttf", "LifeCraft46", 46);
		Load("fonts/LifeCraft_Font.ttf", "LifeCraft80", 80);
		Load("fonts/LifeCraft_Font.ttf", "LifeCraft90", 90);
		

	}

	return ret;
}

// Called before quitting
bool Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");

	std::map<char*, TTF_Font*>::iterator it = named_fonts.begin();
	for (it = named_fonts.begin(); it != named_fonts.end(); ++it)
	{
		TTF_CloseFont(it->second);
	}
	named_fonts.clear();

	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const Fonts::Load(const char* path, char* name, int size)
{
	
	SDL_RWops* buffer = App->fs->Load(path);
	TTF_Font* font = TTF_OpenFontRW(buffer, 1, size); //That closes the buffer automatically

	if(font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		
		named_fonts.insert(std::pair<char*, TTF_Font*>(name, font));
	}
	return font;
}

// Print text using font
SDL_Texture* Fonts::Print(const char* text, SDL_Color color, _TTF_Font* font, uint multilabelwidth)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped((font) ? font : default, text, color, multilabelwidth);

	if(surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->textures->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool Fonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if(TTF_SizeText((font) ? font : default, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}

TTF_Font* Fonts::getFontbyName(char* name) const
{
	return named_fonts.at(name);
}

