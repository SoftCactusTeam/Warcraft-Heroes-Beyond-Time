#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "SDL_image\include\SDL_image.h"
#include "EntitiesEnums.h"
#include "Animation.h"

class Collider;

class Entity {
public:

	enum class EntityType
	{
		NO_TYPE = -1,
		DYNAMIC_ENTITY,
		STATIC_ENTITY

	} entityType = EntityType::NO_TYPE;

	Entity() {}
	Entity(fPoint coor, SDL_Texture* texture, EntityType entityType);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool Finish();
	virtual bool Draw();

	virtual void OnCollision(Collider* yours, Collider* collideWith) {}
	virtual void OnCollisionContinue(Collider* yours, Collider* collideWith) {}
	virtual void OnCollisionLeave(Collider* yours, Collider* collideWith) {}

	void StopConcreteTime(int time);
public:
	fPoint pos = { 0,0 };
	SDL_Texture* texture = nullptr;
	Animation* anim;
	int accountantPrincipal = -1;
	bool destroy = false;
	bool stop = false;

	//to_delete
	bool isGuldan = false;
	bool ballsOnTheAir = false;
};

#endif