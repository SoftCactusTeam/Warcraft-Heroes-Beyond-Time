#ifndef __ModuleColliders_H__
#define __ModuleColliders_H__

#include "Module.h"
#include "Entity.h"
#include <vector>

struct Collider
{
	Collider(Entity* owner, SDL_Rect colliderRect);
	SDL_Rect colliderRect;
	Entity* owner;
};

class ModuleColliders : public Module
{
	bool Update();
	bool CleanUp();

	void AddCollider(Collider col);
	void CleanCollidersEntity(Entity* entity);

private:
	std::vector<Collider*> colliders;
};


#endif