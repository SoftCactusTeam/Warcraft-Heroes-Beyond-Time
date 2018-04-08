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
	printColliders = false;
	return true;
}

bool ModuleColliders::Update(float dt)
{
	BROFILER_CATEGORY("Colliders Collision", Profiler::Color::Azure);

	std::list<Collider*>::iterator first, second = colliders.begin();
	advance(second, 1);
	
	//Check all non_temporal colliders between them
	for (first = colliders.begin(); first != colliders.end(); ++first)
		if ((*first)->type != COLLIDER_UNWALKABLE)
			for (second; second != colliders.end(); ++second)
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
	if (temporalColliders.size() > 0)
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
	std::vector<int>::iterator timer_it = temporalColliderstimer.begin();
	for (second = temporalColliders.begin(); second != temporalColliders.end();)
	{
		if (*(timer_it) < SDL_GetTicks())
		{
			second = temporalColliders.erase(second);
			timer_it = temporalColliderstimer.erase(timer_it);
		}
		else
		{
			++timer_it;
			++second;
		}
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
	std::list<Collider*>::iterator it;
	for (it = colliders.begin(); it != colliders.end(); ++it)
	{
		delete (*it);
	}
	colliders.clear();

	for (it = temporalColliders.begin(); it != temporalColliders.end(); ++it)
	{
		delete (*it);
	}
	temporalColliders.clear();

	temporalColliderstimer.clear();

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
	std::list<Collider*>::iterator it;
	for (it = colliders.begin(); it != colliders.end(); ++it)
	{
		if ((*it) == col)
		{
			colliders.erase(it);
			break;
		}
	}
}

void ModuleColliders::CleanCollidersEntity(Entity* entity)
{
	std::list<Collider*>::iterator it;
	for (it = colliders.begin(); it != colliders.end(); ++it)
		if ((*it)->owner == entity)
		{
			colliders.erase(it);
			break;
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
		std::list<Collider*>::iterator it;

		for (it = colliders.begin(); it != colliders.end(); ++it)
			if ((*it)->owner != nullptr)
				App->render->DrawQuad({ (int)(*it)->owner->pos.x + (*it)->colliderRect.x, (int)(*it)->owner->pos.y + (*it)->colliderRect.y, (*it)->colliderRect.w, (*it)->colliderRect.h }, 255, 255, 255, 100);
			else
				App->render->DrawQuad({ (*it)->colliderRect.x, (*it)->colliderRect.y, (*it)->colliderRect.w, (*it)->colliderRect.h }, 255, 150, 255, 100);

		for (it = temporalColliders.begin(); it != temporalColliders.end(); ++it)
			App->render->DrawQuad({ (*it)->colliderRect.x, (*it)->colliderRect.y, (*it)->colliderRect.w, (*it)->colliderRect.h }, 255, 0, 255, 100);
	}
}

void ModuleColliders::AddCommands()
{
	ConsoleOrder* order = new ConsoleColliders();
	App->console->AddConsoleOrderToList(order);
}