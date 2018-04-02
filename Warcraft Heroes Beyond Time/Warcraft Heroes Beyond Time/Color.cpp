#include "Globals.h"

SDL_Color Red = { 255, 0, 0, 255 };
SDL_Color Green = { 0, 255, 0, 255 };
SDL_Color Blue = { 0, 0, 255, 255 };
SDL_Color White = { 255, 255, 255, 255 };
SDL_Color Black = { 0, 0, 0, 255 };

SDL_Color Grey = { 192, 192, 192, 255 };
SDL_Color Yellow = { 255, 255, 0, 255 };

bool ColorEquals(SDL_Color color1, SDL_Color color2)
{
	return color1.r == color2.r
		&& color1.g == color2.g
		&& color1.b == color2.b
		&& color1.a == color2.a;
}