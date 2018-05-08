#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"
#include "p2Defs.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window();
	virtual ~Window();

	bool Awake(pugi::xml_node&);
	bool CleanUp();

	void SetTitle(const char* new_title);
	void GetWindowSize(uint& width, uint& height) const;
	uint GetScale() const;

public:

	SDL_Window* window;

	SDL_Surface* screen_surface;
private:
	std::string	title;
	std::string	icon;

	uint		width = 0;
	uint		height = 0;
	uint		scale = 0;
};

#endif