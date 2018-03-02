#ifndef __Entity_H__
#define __Entity_H__

#include "p2Point.h"
#include "SDL_image\include\SDL_image.h"
#include "EntitiesEnums.h"

class Entity {
public:
	Entity() {}
	Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	virtual void Start() = 0;
	virtual void Finish() = 0;
	virtual void Draw() = 0;

public:
	iPoint coor;
	SDL_Texture* texture;
	ENTITY_TYPE entityType;
	bool destroy = false;

};

class DinamicEntity : public Entity {
public:
	DinamicEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	virtual void Start() = 0;
	virtual void Finish() = 0;
	virtual void Draw() = 0;

	virtual void Update() = 0;
	virtual void Move() = 0;
	FIXED_ANGLE CaculateAngle(iPoint objectiveCoor);
	void ReceivAtac(int damage, FIXED_ANGLE angle);

public:
	int live = -1;
	int movementSpeed = -1;
	int atacSpeed = -1;
	int accountantPrincipal = -1;
};

class StaticEntity : public Entity {
public:
	StaticEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	virtual void Start() = 0;
	virtual void Finish() = 0;
	virtual void Draw() = 0;

	virtual void Interaction() = 0;
};

#endif