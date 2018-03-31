#ifndef __FONTS_H__
#define __FONTS_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#include "SDL_ttf\include\SDL_ttf.h"

#define DEFAULT_FONT "arial.ttf"
#define DEFAULT_FONT_SIZE 16

#include <map>
#include <iostream>

class SDL_Texture;

class Fonts : public Module
{
public:

	Fonts();

	// Destructor
	virtual ~Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, char* name , int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL, uint multilabelwidth = 500);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

	TTF_Font* getFontbyName(char* name) const;

public:

	std::map<char*, _TTF_Font*> named_fonts;

	TTF_Font*			default;
};


#endif // __j1FONTS_H__