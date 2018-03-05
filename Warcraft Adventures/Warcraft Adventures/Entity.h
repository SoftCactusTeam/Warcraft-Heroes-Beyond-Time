#ifndef __Entity_H__
#define __Entity_H__

#include "p2Point.h"
#include "SDL_image\include\SDL_image.h"
#include "EntitiesEnums.h"

class Entity {
public:
	Entity() {}
	Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	virtual void Start();
	virtual void Update(float dt);
	virtual void Finish();
	void Draw();

public:
	iPoint coor = { 0,0 };
	SDL_Texture* texture = nullptr;
	SDL_Rect rectToPrint = { 0,0,0,0 };
	ENTITY_TYPE entityType = ENTITY_TYPE::
	bool destroy = false;

};



class DynamicEntity : public Entity {
public:
	DynamicEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();

	FIXED_ANGLE CaculateAngle(iPoint objectiveCoor);

public:
	int live = -1;
	int movementSpeed = -1;
	int atacSpeed = -1;
	int accountantPrincipal = -1;
};



class StaticEntity : public Entity {
public:
	StaticEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	virtual void Start();
	virtual void Finish();

	void Update() {};

	virtual void Interaction();
};

#endif