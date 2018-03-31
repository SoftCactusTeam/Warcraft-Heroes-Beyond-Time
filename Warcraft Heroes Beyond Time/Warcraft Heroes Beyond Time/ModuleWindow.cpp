#include "p2Defs.h"
#include "Log.h"

#include "Application.h"
#include "ModuleWindow.h"

#include "SDL/include/SDL.h"


Window::Window() : Module()
{
	window = NULL;
	screen_surface = NULL;
	name = "window";
}

Window::~Window() {}

bool Window::Awake(pugi::xml_node& windowNode)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	icon_surface = SDL_LoadBMP(icon.data());

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = windowNode.child("fullscreen").attribute("active").as_bool();
		bool borderless = windowNode.child("borderless").attribute("active").as_bool();
		bool resizable = windowNode.child("resizable").attribute("active").as_bool();
		bool fullscreen_window = windowNode.child("fullscreen_window").attribute("active").as_bool();

		width = windowNode.child("size").attribute("w").as_uint();
		height = windowNode.child("size").attribute("h").as_uint();

		scale = 1;

		if (fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (fullscreen_window == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(App->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		SDL_SetWindowIcon(window, icon_surface);
		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
	return true;
}

void Window::SetTitle(const char* new_title)
{
	SDL_SetWindowTitle(window, new_title);
}

void Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

uint Window::GetScale() const
{
	return scale;
}

