#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "SDL_image\include\SDL_image.h"
#include "EntitiesEnums.h"

class Entity {
public:
	Entity() {}
	Entity(iPoint coor, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();
	virtual bool Draw(float dt);

public:
	iPoint pos = { 0,0 };
	SDL_Texture* texture = nullptr;
	SDL_Rect rectToPrint = { 0,0,0,0 };
	bool destroy = false;

};

#endif