#ifndef __ModuleColliders_H__
#define __ModuleColliders_H__

#include "Module.h"
#include "Entity.h"
#include "EntitiesEnums.h"
#include <vector>

struct Collider
{
	Collider(Entity* owner, SDL_Rect colliderRect);
	SDL_Rect colliderRect;
	Entity* owner;
	COLLIDER_TYPE type;
};

class ModuleColliders : public Module
{
	bool Update();
	bool CleanUp();

	void AddCollider(Collider col);
	void CleanCollidersEntity(Entity* entity);
	bool CheckCollision(int col1, int col2);

private:
	std::vector<Collider*> colliders;
};


#endif