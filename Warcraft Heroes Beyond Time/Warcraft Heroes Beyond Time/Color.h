#ifndef __COLOR_H__
#define __COLOR_H__

struct Color
{
	Color(int r = 255, int g = 255, int b = 255, int a = 255) : r(r), g(g), b(b), a(a) {}

	int r; 
	int g;
	int b;
	int a;
};



#endif