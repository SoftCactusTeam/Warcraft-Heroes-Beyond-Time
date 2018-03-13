#include "ModuleColliders.h"

Collider::Collider(Entity* owner, SDL_Rect colliderRect)
{
	this->owner = owner;
	this->colliderRect = colliderRect;
}

bool ModuleColliders::Update()
{
	for (int i = 0; i < colliders.size(); i++)
	{
		for (int col = i + 1; col < colliders.size(); col++)
		{
			if (CheckCollision(i,col))
			{
				colliders[i]->owner->Collision(colliders[col]->type);
				colliders[col]->owner->Collision(colliders[i]->type);
			}
		}
	}

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

bool ModuleColliders::CheckCollision(int col1, int col2)
{
	return (colliders[col1]->colliderRect.x < colliders[col2]->colliderRect.x + colliders[col2]->colliderRect.w &&
		colliders[col1]->colliderRect.x + colliders[col1]->colliderRect.w > colliders[col2]->colliderRect.x &&
		colliders[col1]->colliderRect.y < colliders[col2]->colliderRect.y + colliders[col2]->colliderRect.h &&
		colliders[col1]->colliderRect.y + colliders[col1]->colliderRect.h > colliders[col2]->colliderRect.y);
}