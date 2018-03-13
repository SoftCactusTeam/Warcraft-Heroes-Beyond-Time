#include "ModuleColliders.h"

// TO PRINT COLLIDERS
#include "Application.h"
#include "ModuleRender.h"

Collider::Collider(Entity* owner, SDL_Rect colliderRect, COLLIDER_TYPE type, iPoint offset)
{
	this->owner = owner;
	this->colliderRect = colliderRect;
	this->type = type;
	this->colliderRect.x = offset.x;
	this->colliderRect.y = offset.y;
}

bool ModuleColliders::Update(float dt)
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
	PrintColliders(printColliders);
	return true;
}

bool ModuleColliders::CleanUp()
{
	for (int i = 0; i < colliders.size(); i++)
		delete colliders[i];
	colliders.clear();
	return true;
}

void ModuleColliders::AddCollider(Entity* owner, SDL_Rect colliderRect, COLLIDER_TYPE type, iPoint offset)
{

	Collider* aux = new Collider(owner, colliderRect, type, offset);
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
	return (colliders[col1]->owner->pos.x + colliders[col1]->colliderRect.x < colliders[col2]->owner->pos.x + colliders[col2]->colliderRect.x + colliders[col2]->colliderRect.w &&
		colliders[col1]->owner->pos.x + colliders[col1]->colliderRect.x + colliders[col1]->colliderRect.w > colliders[col2]->owner->pos.x + colliders[col2]->colliderRect.x &&
		colliders[col1]->owner->pos.y + colliders[col1]->colliderRect.y < colliders[col2]->owner->pos.y + colliders[col2]->colliderRect.y + colliders[col2]->colliderRect.h &&
		colliders[col1]->owner->pos.y + colliders[col1]->colliderRect.y + colliders[col1]->colliderRect.h > colliders[col2]->owner->pos.y + colliders[col2]->colliderRect.y);
}

void ModuleColliders::PrintColliders(bool print)
{
	if (print)
		for (int i = 0; i < colliders.size(); i++)
			App->render->DrawQuad({ colliders[i]->owner->pos.x + colliders[i]->colliderRect.x, colliders[i]->owner->pos.y + colliders[i]->colliderRect.y, colliders[i]->colliderRect.w, colliders[i]->colliderRect.h }, 255, 255, 255, 100);
}