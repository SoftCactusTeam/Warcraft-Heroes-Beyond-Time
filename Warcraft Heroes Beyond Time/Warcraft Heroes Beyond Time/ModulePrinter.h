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

	iPoint pos;
	SDL_Texture* texture;
	SDL_Rect SquaretoBlit;

public:

	Sprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit) : pos(pos), texture(texture), SquaretoBlit(SquaretoBlit) {}

};

class Compare
{
public:
	bool operator () (Sprite* first, Sprite* second)
	{
		//Less to high order 
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

	bool PrintSprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit);

private:

	bool isLower(Sprite* first, Sprite* second) const
	{
		return first->pos.y + first->SquaretoBlit.h < second->pos.y + second->SquaretoBlit.h;
	}

private:

	std::priority_queue<Sprite*, std::vector<Sprite*>, Compare> SpriteQueue;
};



#endif