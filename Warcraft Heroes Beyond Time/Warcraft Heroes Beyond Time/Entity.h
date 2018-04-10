#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "SDL_image\include\SDL_image.h"
#include "EntitiesEnums.h"
#include "Animation.h"

class Entity {
public:
	Entity() {}
	Entity(fPoint coor, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool Finish();
	virtual bool Draw();

	virtual void Collision(COLLIDER_TYPE type);
	void StopConcreteTime(int time);
public:
	fPoint pos = { 0,0 };
	SDL_Texture* texture = nullptr;
	Animation* anim;
	int accountantPrincipal = -1;
	bool destroy = false;
	bool stop = false;
};

#endif