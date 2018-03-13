#include "ModuleColliders.h"

Collider::Collider(Entity* owner, SDL_Rect colliderRect)
{
	this->owner = owner;
	this->colliderRect = colliderRect;
}

bool ModuleColliders::Update()
{

	return true;
}

bool ModuleColliders::CleanUp()
{
	for (int i = 0; i < colliders.size(); i++)
		delete colliders[i];
	colliders.clear();
	return true;
}

void ModuleColliders::AddCollider(Collider col)
{
	Collider* aux = new Collider(col);
	colliders.push_back(aux);
}

void ModuleColliders::CleanCollidersEntity(Entity* entity)
{
	for (int i = 0; i < colliders.size(); i++)
		if (colliders[i]->owner == entity)
		{
			delete colliders[i];
			std::swap(colliders[i], colliders.back());
			colliders.pop_back();
		}
}