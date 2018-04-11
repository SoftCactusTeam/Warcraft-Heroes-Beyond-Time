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
<<<<<<< HEAD
	ConsoleOrder* order = new ConsoleColliders();
	App->console->AddConsoleOrderToList(order);
=======
	return true;
}

bool ModuleColliders::Start()
{
>>>>>>> master
	printColliders = false;
	return true;
}

bool ModuleColliders::Update(float dt)
{
	BROFILER_CATEGORY("Colliders Collision", Profiler::Color::Azure);

	for (int i = 0; i < colliders.size(); i++)
		if (colliders[i]->type != COLLIDER_UNWALKABLE)
			for (int col = i + 1; col < colliders.size(); col++)
				if (CheckTypeCollMatrix(colliders[i]->type, colliders[col]->type))
					if (CheckCollision(i, col))
					{
						if (colliders[i]->owner != nullptr)
							colliders[i]->owner->Collision(colliders[col]->type);
						else
							colliders[i]->collidingWith = colliders[col]->type;	// Aixo es quan el collider no te entity pero vol detectar

						if (colliders[col]->owner != nullptr)
							colliders[col]->owner->Collision(colliders[i]->type);
						else
							colliders[col]->collidingWith = colliders[i]->type;
					}
	// Comprobar colliders temporals
	for (int i = 0; i < colliders.size(); i++)
		for (int col = 0; col < temporalColliders.size(); col++)
			if (CheckTypeCollMatrix(colliders[i]->type, colliders[col]->type))
				if (ChechCollisionTemporalCollider(i, col))
				{
					if (colliders[i]->owner != nullptr)
						colliders[i]->owner->Collision(temporalColliders[col]->type);
					else
						colliders[i]->collidingWith = temporalColliders[col]->type;
				}
	// Netejar colliders temporals
	for (int i = 0; i < temporalColliders.size(); i++)
		if (temporalColliderstimer[i] < SDL_GetTicks())
		{
			delete temporalColliders[i];
			std::swap(temporalColliders[i], temporalColliders.back());
			temporalColliders.pop_back();

			std::swap(temporalColliderstimer[i], temporalColliderstimer.back());
			temporalColliderstimer.pop_back();
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
<<<<<<< HEAD
	for (int i = 0; i < colliders.size(); i++)
		if (colliders[i]->owner == entity)
		{
			delete colliders[i];
			colliders.erase(colliders.begin() + i);
		}
=======
	if (entity != nullptr) 
	{
		std::list<Collider*>::iterator it;
		for (it = colliders.begin(); it != colliders.end(); ++it)
			if ((*it)->owner == entity)
			{
				delete (*it);
				colliders.erase(it);
				break;
			}
	}
>>>>>>> master
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
<<<<<<< HEAD
	case COLLIDER_ENEMY_ATAC:
		if (type2 == COLLIDER_PLAYER || type2 == COLLIDER_PLAYER_ATAC)
=======
	case COLLIDER_ENEMY_ATTACK:
		if (type2 == COLLIDER_PLAYER || type2 == COLLIDER_PLAYER_ATTACK || type2 == COLLIDER_UNWALKABLE)
>>>>>>> master
			return true;
		break;
	}
	return false;
}

bool ModuleColliders::CheckCollision(int col1, int col2)
{
	if (colliders[col1]->owner != nullptr && colliders[col2]->owner != nullptr)
		return (colliders[col1]->owner->pos.x + colliders[col1]->colliderRect.x < colliders[col2]->owner->pos.x + colliders[col2]->colliderRect.x + colliders[col2]->colliderRect.w &&
			colliders[col1]->owner->pos.x + colliders[col1]->colliderRect.x + colliders[col1]->colliderRect.w > colliders[col2]->owner->pos.x + colliders[col2]->colliderRect.x &&
			colliders[col1]->owner->pos.y + colliders[col1]->colliderRect.y < colliders[col2]->owner->pos.y + colliders[col2]->colliderRect.y + colliders[col2]->colliderRect.h &&
			colliders[col1]->owner->pos.y + colliders[col1]->colliderRect.y + colliders[col1]->colliderRect.h > colliders[col2]->owner->pos.y + colliders[col2]->colliderRect.y);
	else if (colliders[col1]->owner != nullptr && colliders[col2]->owner == nullptr)
		return (colliders[col1]->owner->pos.x + colliders[col1]->colliderRect.x < colliders[col2]->colliderRect.x + colliders[col2]->colliderRect.w &&
			colliders[col1]->owner->pos.x + colliders[col1]->colliderRect.x + colliders[col1]->colliderRect.w > colliders[col2]->colliderRect.x &&
			colliders[col1]->owner->pos.y + colliders[col1]->colliderRect.y < colliders[col2]->colliderRect.y + colliders[col2]->colliderRect.h &&
			colliders[col1]->owner->pos.y + colliders[col1]->colliderRect.y + colliders[col1]->colliderRect.h > colliders[col2]->colliderRect.y);
	else if (colliders[col1]->owner == nullptr && colliders[col2]->owner != nullptr)
		return (colliders[col1]->colliderRect.x < colliders[col2]->owner->pos.x + colliders[col2]->colliderRect.x + colliders[col2]->colliderRect.w &&
			colliders[col1]->colliderRect.x + colliders[col1]->colliderRect.w > colliders[col2]->owner->pos.x + colliders[col2]->colliderRect.x &&
			colliders[col1]->colliderRect.y < colliders[col2]->owner->pos.y + colliders[col2]->colliderRect.y + colliders[col2]->colliderRect.h &&
			colliders[col1]->colliderRect.y + colliders[col1]->colliderRect.h > colliders[col2]->owner->pos.y + colliders[col2]->colliderRect.y);
}

bool ModuleColliders::ChechCollisionTemporalCollider(int col, int colTemporal)
{
	if (colliders[col]->owner != nullptr)
	return (colliders[col]->owner->pos.x + colliders[col]->colliderRect.x < temporalColliders[colTemporal]->colliderRect.x + temporalColliders[colTemporal]->colliderRect.w &&
		colliders[col]->owner->pos.x + colliders[col]->colliderRect.x + colliders[col]->colliderRect.w > temporalColliders[colTemporal]->colliderRect.x &&
		colliders[col]->owner->pos.y + colliders[col]->colliderRect.y < temporalColliders[colTemporal]->colliderRect.y + temporalColliders[colTemporal]->colliderRect.h &&
		colliders[col]->owner->pos.y + colliders[col]->colliderRect.y + colliders[col]->colliderRect.h > temporalColliders[colTemporal]->colliderRect.y);
	else if (colliders[col]->owner == nullptr)
		return (colliders[col]->colliderRect.x < temporalColliders[colTemporal]->colliderRect.x + temporalColliders[colTemporal]->colliderRect.w &&
			colliders[col]->colliderRect.x + colliders[col]->colliderRect.w > temporalColliders[colTemporal]->colliderRect.x &&
			colliders[col]->colliderRect.y < temporalColliders[colTemporal]->colliderRect.y + temporalColliders[colTemporal]->colliderRect.h &&
			colliders[col]->colliderRect.y + colliders[col]->colliderRect.h > temporalColliders[colTemporal]->colliderRect.y);
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