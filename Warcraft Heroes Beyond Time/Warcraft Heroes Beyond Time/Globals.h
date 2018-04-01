#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "SDL/include/SDL.h"

#define PI 3.14159265358979323846f
#define RAD_2_DEG(angle) angle * 180.0f / PI
#define DEG_2_RAD(angle) angle * PI / 180.0f

typedef unsigned int uint;

extern SDL_Color Red;
extern SDL_Color Green;
extern SDL_Color Blue;
extern SDL_Color Black;
extern SDL_Color White;
extern SDL_Color Grey;
extern SDL_Color Yellow;

bool ColorEquals(SDL_Color color1, SDL_Color color2);

#endif