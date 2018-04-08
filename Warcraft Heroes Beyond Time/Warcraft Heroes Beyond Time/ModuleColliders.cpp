#include "ModuleColliders.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Console.h"

#include "Brofiler\Brofiler.h"

class ConsoleColliders : public ConsoleOrder
{
	std::string orderName() { return "colliders"; }
	void Exec(std::string parametre, int parametreNumeric) {
		if (parametre == "print")
			if (parametreNumeric == 1)
				App->colliders->printColliders = true;
			else if (parametreNumeric == 0)
				App->colliders->printColliders = false;
	}
};

Collider::Collider(SDL_Rect colliderRect, COLLIDER_TYPE type, Entity* owner, iPoint offset)
{
	this->owner = owner;
	this->colliderRect = colliderRect;
	this->type = type;
	this->colliderRect.x += offset.x;
	this->colliderRect.y += offset.y;
}

ModuleColliders::ModuleColliders()
{
	name = "colliders";
}

bool ModuleColliders::Awake(pugi::xml_node& consoleNode)
{
	ConsoleOrder* order = new ConsoleColliders();
	App->console->AddConsoleOrderToList(order);
	printColliders = false;
	return true;
}

bool ModuleColliders::Update(float dt)
{
	BROFILER_CATEGORY("Colliders Collision", Profiler::Color::Azure);

	std::list<Collider*>::iterator first;
	std::list<Collider*>::iterator second;

	for (first = colliders.begin(); first != colliders.end(); ++first)
		if ((*first)->type != COLLIDER_UNWALKABLE)
			for (second = colliders.begin()++; second != colliders.end(); ++second)
				if (CheckTypeCollMatrix((*first)->type, (*second)->type))
					if (CheckCollision((*first), (*second)))
					{
						if ((*first) != nullptr)
							(*first)->owner->Collision((*second)->type);
						else
							(*first)->collidingWith = (*second)->type;	// Aixo es quan el collider no te entity pero vol detectar

						if ((*second)->owner != nullptr)
							(*second)->owner->Collision((*first)->type);
						else
							(*second)->collidingWith = (*first)->type;
					}

	// Comprobar colliders temporals
	for (first = colliders.begin(); first != colliders.end(); ++first)
		for (second = temporalColliders.begin(); second != colliders.end(); ++second)
			if (CheckTypeCollMatrix((*first)->type, (*second)->type))
				if (CheckCollision((*first), (*second)))
				{
					if ((*first)->owner != nullptr)
						(*first)->owner->Collision((*second)->type);
					else
						(*first)->collidingWith = (*second)->type;
				}

	// Netejar colliders temporals
	int i = 0;
	for (second = temporalColliders.begin(); second != temporalColliders.end(); second++, ++i)
		if (temporalColliderstimer[i] < SDL_GetTicks())
		{
			temporalColliders.erase(second);
			temporalColliderstimer.erase(temporalColliderstimer.begin() + i);
		}
	return true;
}

bool ModuleColliders::PostUpdate()
{
	PrintColliders();
	return true;
}

bool ModuleColliders::CleanUp()
{
	for (int i = 0; i < colliders.size(); i++)
		delete colliders[i];
	colliders.clear();
	return true;
}

Collider* ModuleColliders::AddCollider(SDL_Rect colliderRect, COLLIDER_TYPE type, Entity* owner, iPoint offset)
{
	if (owner != nullptr)
	{
		Collider* aux = new Collider(colliderRect, type, owner, offset);
		colliders.push_back(aux);
		return aux;
	}
	else
	{
		Collider* aux = new Collider(colliderRect, type);
		colliders.push_back(aux);
		return aux;
	}
}

Collider* ModuleColliders::AddTemporalCollider(SDL_Rect colliderRect, COLLIDER_TYPE type, int timer)
{
	Collider* aux = new Collider(colliderRect, type);
	temporalColliders.push_back(aux);
	temporalColliderstimer.push_back(timer + SDL_GetTicks());
	return aux;
}

void ModuleColliders::deleteCollider(Collider* col)
{
	for (int i = 0; i < colliders.size(); i++)
		if (colliders[i] == col)
		{
			delete colliders[i];
			colliders.erase(colliders.begin() + i);
		}
}

void ModuleColliders::CleanCollidersEntity(Entity* entity)
{
	for (int i = 0; i < colliders.size(); i++)
		if (colliders[i]->owner == entity)
		{
			delete colliders[i];
			colliders.erase(colliders.begin() + i);
		}
}

bool ModuleColliders::CheckTypeCollMatrix(COLLIDER_TYPE type, COLLIDER_TYPE type2)
{
	switch (type)
	{
	case COLLIDER_PLAYER:
		if (type2 == COLLIDER_ENEMY || type2 == COLLIDER_ENEMY_ATAC || type2 == COLLIDER_WALKABLE)
			return true;
		break;
	case COLLIDER_ENEMY:
		if (type2 == COLLIDER_ENEMY || type2 == COLLIDER_PLAYER_ATAC || type2 == COLLIDER_PLAYER)
			return true;
		break;
	case COLLIDER_PLAYER_ATAC:
		if (type2 == COLLIDER_ENEMY || type2 == COLLIDER_ENEMY_ATAC)
			return true;
		break;
	case COLLIDER_ENEMY_ATAC:
		if (type2 == COLLIDER_PLAYER || type2 == COLLIDER_PLAYER_ATAC)
			return true;
		break;
	}
	return false;
}

bool ModuleColliders::CheckCollision(Collider* col1, Collider* col2)
{
	if (col1->owner != nullptr && col2->owner != nullptr)

		return (col1->owner->pos.x + col1->colliderRect.x < col2->owner->pos.x + col2->colliderRect.x + col2->colliderRect.w &&
			col1->owner->pos.x + col1->colliderRect.x + col2->colliderRect.w > col2->owner->pos.x + col2->colliderRect.x &&
			col1->owner->pos.y + col1->colliderRect.y < col2->owner->pos.y + col2->colliderRect.y + col2->colliderRect.h &&
			col1->owner->pos.y + col1->colliderRect.y + col1->colliderRect.h > col2->owner->pos.y + col2->colliderRect.y);

	else if (col1->owner != nullptr && col2->owner == nullptr)
		return (col1->owner->pos.x + col1->colliderRect.x < col2->colliderRect.x + col2->colliderRect.w &&
			col1->owner->pos.x + col1->colliderRect.x + col1->colliderRect.w > col2->colliderRect.x &&
			col1->owner->pos.y + col1->colliderRect.y < col2->colliderRect.y + col2->colliderRect.h &&
			col1->owner->pos.y + col1->colliderRect.y + col1->colliderRect.h > col2->colliderRect.y);

	else if (col1->owner == nullptr && col2->owner != nullptr)
		return (col1->colliderRect.x < col2->owner->pos.x + col2->colliderRect.x + col2->colliderRect.w &&
			col1->colliderRect.x + col1->colliderRect.w > col2->owner->pos.x + col2->colliderRect.x &&
			col1->colliderRect.y < col2->owner->pos.y + col2->colliderRect.y + col2->colliderRect.h &&
			col1->colliderRect.y + col1->colliderRect.h > col2->owner->pos.y + col2->colliderRect.y);

	else if (col1->owner == nullptr && col2->owner == nullptr)
		return (col1->colliderRect.x < col2->colliderRect.x + col2->colliderRect.w &&
			col1->colliderRect.x + col1->colliderRect.w > col2->colliderRect.x &&
			col1->colliderRect.y < col2->colliderRect.y + col2->colliderRect.h &&
			col1->colliderRect.y + col1->colliderRect.h > col2->colliderRect.y);
}

void ModuleColliders::PrintColliders()
{
	if (printColliders)
	{
		for (int i = 0; i < colliders.size(); i++)
			if (colliders[i]->owner != nullptr)
				App->render->DrawQuad({ (int)colliders[i]->owner->pos.x + colliders[i]->colliderRect.x, (int)colliders[i]->owner->pos.y + colliders[i]->colliderRect.y, colliders[i]->colliderRect.w, colliders[i]->colliderRect.h }, 255, 255, 255, 100);
			else
				App->render->DrawQuad({colliders[i]->colliderRect.x, colliders[i]->colliderRect.y, colliders[i]->colliderRect.w, colliders[i]->colliderRect.h }, 255, 150, 255, 100);

		for (int i = 0; i < temporalColliders.size(); i++)
			App->render->DrawQuad({ temporalColliders[i]->colliderRect.x, temporalColliders[i]->colliderRect.y, temporalColliders[i]->colliderRect.w, temporalColliders[i]->colliderRect.h }, 255, 0, 255, 100);
	}
}