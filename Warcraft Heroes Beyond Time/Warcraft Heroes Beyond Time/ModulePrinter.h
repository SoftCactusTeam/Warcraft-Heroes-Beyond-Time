#ifndef __MODULEPRINTER_H__
#define __MODULEPRINTER_H__

#include "Module.h"
#include "Application.h"
#include "p2Point.h"

#include <queue>

class SDL_Texture;


class Sprite
{
public:

	iPoint pos = { 0,0 };
	SDL_Texture* texture = nullptr;
	SDL_Rect SquaretoBlit;
	int layer = 0;
	iPoint pivot;
	float angle;

public:

	Sprite(iPoint& pos, SDL_Texture* texture, SDL_Rect& SquaretoBlit, int layer, iPoint pivot, float angle) : pos(pos), texture(texture), SquaretoBlit(SquaretoBlit), layer(layer), pivot(pivot), angle(angle){}

};

class Compare
{
public:
	bool operator () (Sprite* first, Sprite* second)
	{
		if (first->layer != second->layer)
			return first->layer > second->layer;
		else
			return first->pos.y + first->SquaretoBlit.h > second->pos.y + second->SquaretoBlit.h;
	}
};


class ModulePrinter : public Module
{
public:

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:

	enum class Pivots
	{
		UPPER_LEFT,
		UPPER_CENTER,
		UPPER_RIGHT,
		CENTER_LEFT,
		CENTER,
		CENTER_RIGHT,
		LOWER_LEFT,
		LOWER_CENTER,
		LOWER_RIGHT,
		CUSTOM_PIVOT
	};

	//Note: Angle required is in degrees, in clockwise direction
	bool PrintSprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit, int layer = 0, Pivots pivot = Pivots::CENTER, float degangle = 0, iPoint custompivot = {0,0});

private:

	std::priority_queue<Sprite*, std::vector<Sprite*>, Compare> SpriteQueue;
};



#endif