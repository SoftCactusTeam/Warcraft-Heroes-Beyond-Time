#include "ModuleColliders.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Console.h"

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

Collider::Collider(SDL_Rect colliderRect, COLLIDER_TYPE type)
{
	this->owner = nullptr;
	this->colliderRect = colliderRect;
	this->type = type;
}

Collider::Collider(Entity* owner, SDL_Rect colliderRect, COLLIDER_TYPE type, iPoint offset)
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
	for (int i = 0; i < colliders.size(); i++)
		for (int col = i + 1; col < colliders.size(); col++)
			if (CheckCollision(i,col))
			{
				colliders[i]->owner->Collision(colliders[col]->type);
				colliders[col]->owner->Collision(colliders[i]->type);
			}
	// Comprobar colliders temporals
	for (int i = 0; i < colliders.size(); i++)
		for (int col = 0; col < temporalColliders.size(); col++)
			if (ChechCollisionTemporalCollider(i, col))
			{
				colliders[i]->owner->Collision(temporalColliders[col]->type);
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

void ModuleColliders::AddTemporalCollider(SDL_Rect colliderRect, COLLIDER_TYPE type, int timer)
{
	Collider* aux = new Collider(nullptr, colliderRect, type, {0,0});
	temporalColliders.push_back(aux);
	temporalColliderstimer.push_back(timer + SDL_GetTicks());
}

void ModuleColliders::AddTileCollider(SDL_Rect colliderRect, COLLIDER_TYPE type)
{
	Collider* aux = new Collider(colliderRect, type);
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

void ModuleColliders::PrintColliders(bool print)
{
	if (print)
	{
		for (int i = 0; i < colliders.size(); i++)
			/*printf_s("aa");*/
			App->render->DrawQuad({ (int)(colliders[i]->owner->pos.x + colliders[i]->colliderRect.x), (int)(colliders[i]->owner->pos.y + colliders[i]->colliderRect.y), colliders[i]->colliderRect.w, colliders[i]->colliderRect.h }, 255, 255, 255, 100);
		for (int i = 0; i < temporalColliders.size(); i++)
			App->render->DrawQuad({ temporalColliders[i]->colliderRect.x, temporalColliders[i]->colliderRect.y, temporalColliders[i]->colliderRect.w, temporalColliders[i]->colliderRect.h }, 255, 0, 255, 100);
	}
}
