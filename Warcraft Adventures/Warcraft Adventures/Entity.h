#ifndef __Entity_H__
#define __Entity_H__

#include "p2Point.h"
#include "SDL_image\include\SDL_image.h"

enum ENTITY_TYPE {
	DINAMIC_PLAYER,
	DINAMIC_BOSS,
	DINAMIC_ENEMIES,
	STATIC_CONSUMABLE,
	STATIC_CHEST,
	STATIC_ESCENE_ITEM
};

enum ENEMY_TYPE {
	FOOTMAN,
	ARCHER,
	MAGE,
	DEATH_KNIGHT,
	GOBLIN,
	ZOMBIE
};

enum PLAYER_TYPE {
	THRALL,
	VALEERA,
	SYLVANAS
};

enum BOSS_TYPE {
	LICH_KING,
	ILLIDAN,
	GULDAN
};

enum CONSUMABLE_TYPE {
	LIVE_POTION,
	ATACK_POTION,
	MOVEMENT_SPEED_POTION
};

enum CHEST_TYPE {
	LOW_CHEST,
	MID_CHEST,
	HIGH_CHEST
};

enum STATIC_OBJECT_TYPE {
	TREE,
	ROCK
};

enum FIXED_ANGLE {
	UP,
	UP_RIGHT,
	RIGHT,
	DOWN_RIGHT,
	DOWN,
	DOWN_LEFT,
	LEFT,
	UP_LEFT
};

class Entity {
public:
	Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	virtual void Start() = 0;
	virtual void Finish() = 0;
	virtual void Draw() = 0;

private:
	bool destroy = false;

public:
	iPoint coor;
	SDL_Texture* texture;
	ENTITY_TYPE entityType;

};

class DinamicEntity : public Entity {
public:
	DinamicEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	virtual void Update() = 0;
	virtual void Move() = 0;
	FIXED_ANGLE CaculateAngle(iPoint objectiveCoor);
	void ReceivAtac(int damage, FIXED_ANGLE angle);

public:
	int live;
	int movementSpeed;
	int atacSpeed;
	int accountantPrincipal;
};


class StaticEntity : public Entity {

};

#endif