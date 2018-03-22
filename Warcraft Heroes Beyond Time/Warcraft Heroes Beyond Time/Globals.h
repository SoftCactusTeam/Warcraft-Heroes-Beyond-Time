#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "SDL/include/SDL.h"

#define PI 3.14159265358979323846f
#define RAD_2_DEG(angle) angle * 180 / PI

typedef unsigned int uint;

extern SDL_Color Red;
extern SDL_Color Green;
extern SDL_Color Blue;
extern SDL_Color Black;
extern SDL_Color White;
extern SDL_Color Grey;

inline bool operator==(const SDL_Rect& a, const SDL_Rect& b)
{
	return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
}


inline bool operator!=(const SDL_Rect& a, const SDL_Rect& b)
{
	if (a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h)
		return false;
	else
		return true;
}

#endif