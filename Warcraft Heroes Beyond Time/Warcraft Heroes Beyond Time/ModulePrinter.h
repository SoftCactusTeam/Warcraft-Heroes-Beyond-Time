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

public:

	Sprite(iPoint& pos, SDL_Texture* texture, SDL_Rect& SquaretoBlit, int layer) : pos(pos), texture(texture), SquaretoBlit(SquaretoBlit), layer(layer) {}

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

	bool PrintSprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit, int layer = 0);

private:

	std::priority_queue<Sprite*, std::vector<Sprite*>, Compare> SpriteQueue;
};



#endif