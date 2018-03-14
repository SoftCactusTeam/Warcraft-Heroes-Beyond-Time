#ifndef __ModuleColliders_H__
#define __ModuleColliders_H__

#include "Module.h"
#include "Entity.h"
#include "EntitiesEnums.h"
#include <vector>

struct Collider
{
	Collider(Entity* owner, SDL_Rect colliderRect, COLLIDER_TYPE type, iPoint offset);
	SDL_Rect colliderRect;	// El X i Y del Rect fan de offset !!!
	Entity* owner;
	COLLIDER_TYPE type;
};

class ModuleColliders : public Module
{
public:
	bool Awake(pugi::xml_node& consoleNode);
	bool Update(float dt);
	bool CleanUp();

	void AddCollider(Entity* owner, SDL_Rect colliderRect, COLLIDER_TYPE type, iPoint offset);
	void AddTemporalCollider(SDL_Rect colliderRect, COLLIDER_TYPE type, int timer);
	void CleanCollidersEntity(Entity* entity);
	bool CheckCollision(int col1, int col2);
	bool ChechCollisionTemporalCollider(int col, int colTemporal);
	void PrintColliders(bool print);

private:
	std::vector<Collider*> colliders;
	// Aquestes 2 llistes van en paralel
	std::vector<Collider*> temporalColliders;
	std::vector<int> temporalColliderstimer;
	//	--------------------------------

public:
	bool printColliders = false;

};


#endif