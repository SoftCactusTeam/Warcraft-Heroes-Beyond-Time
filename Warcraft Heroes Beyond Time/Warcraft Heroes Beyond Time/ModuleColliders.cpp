#include "ModuleColliders.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Console.h"
#include "Entity.h"
#include "DynamicEntity.h"
#include "ModulePrinter.h"

#include "Brofiler\Brofiler.h"





#include "PlayerEntity.h"

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

ModuleColliders::ModuleColliders()
{
	name = "colliders";
}

bool ModuleColliders::Awake(pugi::xml_node& consoleNode)
{
	return true;
}

bool ModuleColliders::Start()
{
	return true;
}

bool ModuleColliders::Update(float dt)
{
	BROFILER_CATEGORY("Colliders Collision", Profiler::Color::Azure);

	std::list<Collider*>::iterator col1;
	std::list<Collider*>::iterator col2;

	for (col1 = colliderList.begin(); col1 != colliderList.end(); ++col1)
	{
		if ((*col1)->colType == Collider::ColliderType::WALL)
			continue;

		for (col2 = colliderList.begin(); col2 != colliderList.end(); ++col2)
		{
			if ((*col1) == (*col2) || !CollisionEnabled((*col1), (*col2)))
				continue;
			
			if (CheckIfCollides((*col1), (*col2)))			//If collides
			{
				if ((*col1)->owner != nullptr)
				{
					Entity* owner = (Entity*)(*col1)->owner; //Owners can only be entities for now
					if (wereColliding((*col1), (*col2)))
						owner->OnCollisionContinue((*col1),(*col2));
					else
					{
						owner->OnCollision((*col1),(*col2));
						(*col1)->colliding.push_back(*col2);
					}	
				}

				if ((*col2)->owner != nullptr)
				{
					Entity* owner = (Entity*)(*col2)->owner; 
					if (wereColliding((*col2), (*col1)))
						owner->OnCollisionContinue((*col2),(*col1));
					else
					{
						owner->OnCollision((*col2),(*col1));
						(*col2)->colliding.push_back(*col1);
					}
				}
			}
			else                                                //If don't collide			
			{
				if(wereColliding((*col1), (*col2)))				//If were colliding

					if ((*col1)->owner != nullptr) 
					{
						Entity* owner = (Entity*)(*col1)->owner; 
						owner->OnCollisionLeave((*col1),(*col2));
						(*col1)->colliding.remove(*col2);		
					}

				if (wereColliding((*col2), (*col1)))

					if ((*col2)->owner != nullptr)
					{
						Entity* owner = (Entity*)(*col2)->owner;
						owner->OnCollisionLeave((*col2),(*col1));
						(*col1)->colliding.remove(*col1);
					}			
			}
		}
	}

	return true;
}

bool ModuleColliders::PostUpdate()
{
	if(printColliders)
		PrintColliders();

	return true;
}

bool ModuleColliders::CleanUp()
{
	std::list<Collider*>::iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		delete (*it);
	}
	colliderList.clear();

	return true;
}

Collider* ModuleColliders::AddCollider(SDL_Rect rectArea, Collider::ColliderType colType, void* owner)
{
	Collider* collider = new Collider(rectArea, colType, owner);
	colliderList.push_back(collider);

	return collider;
}

Collider* ModuleColliders::AddPlayerAttackCollider(SDL_Rect rectArea, void* owner, float damage, PlayerAttack::P_Attack_Type pattacktype)
{
	PlayerAttack* playerattack = new PlayerAttack(rectArea, Collider::ColliderType::PLAYER_ATTACK, owner, damage, pattacktype);
	colliderList.push_back(playerattack);

	return playerattack;
}

Collider* ModuleColliders::AddEnemyAttackCollider(SDL_Rect rectArea, void* owner, float damage, EnemyAttack::E_Attack_Type eattacktype)
{
	EnemyAttack* enemyattack = new EnemyAttack(rectArea, Collider::ColliderType::ENEMY_ATTACK, owner, damage, eattacktype);
	colliderList.push_back(enemyattack);

	return enemyattack;
}

void ModuleColliders::deleteCollider(Collider* col)
{
	std::list<Collider*>::iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		if ((*it) == col)
		{
			delete(*it);
			colliderList.erase(it);
			break;
		}
	}
}

void ModuleColliders::deleteColliderbyOwner(void* owner)
{
	std::list<Collider*>::iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		if ((*it)->owner == owner)
		{
			delete (*it);
			colliderList.erase(it);
			break;
		}	
	}
}

bool ModuleColliders::CheckIfCollides(Collider* col1, Collider* col2) const
{
	SDL_Rect rect1 = col1->rectArea;
	if (col1->owner != nullptr)
	{
		switch (col1->colType)
		{
			case Collider::ColliderType::ENTITY:
			{
				Entity* owner = (Entity*)col1->owner;
				rect1.x += owner->pos.x;
				rect1.y += owner->pos.y;
				break;
			}
		}
	}

	SDL_Rect rect2 = col2->rectArea;
	if (col2->owner != nullptr)
	{
		switch (col2->colType)
		{
			case Collider::ColliderType::ENTITY:
			{
				Entity* owner = (Entity*)col2->owner;
				rect2.x += owner->pos.x;
				rect2.y += owner->pos.y;
				break;
			}
		}
	}

	return (rect1.x < rect2.x + rect2.w && rect1.x + rect1.w > rect2.x &&
			rect1.y < rect2.y + rect2.h && rect1.y + rect1.h > rect2.y);

}

bool ModuleColliders::CollisionEnabled(Collider* col1, Collider* col2) const 
{
	switch (col1->colType)
	{
		case Collider::ColliderType::ENTITY:
		{
			Entity* owner1 = (Entity*)col1->owner;
			switch (col2->colType)
			{
				case Collider::ColliderType::ENTITY:
				{
					if (col1->owner == col2->owner)
						return false;
					else
						return true;														//2 different entities enabled
					break;
				}
				case Collider::ColliderType::ENEMY_ATTACK:
				{
					if (owner1->entityType == Entity::EntityType::DYNAMIC_ENTITY)
					{
						DynamicEntity* dynOwner = (DynamicEntity*)owner1;
						if (dynOwner->dynamicType == DynamicEntity::DynamicType::PLAYER)
							return true;													//Player vs Enemy Attack enabled
						else
							return false;
					}
					break;
				}
				case Collider::ColliderType::PLAYER_ATTACK:
				{
					if (owner1->entityType == Entity::EntityType::DYNAMIC_ENTITY)
					{
						DynamicEntity* dynOwner = (DynamicEntity*)owner1;
						if (dynOwner->dynamicType == DynamicEntity::DynamicType::ENEMY)
							return true;													//Enemy vs Player Attack enabled
						else
							return false;
					}
					break;
				}
				case Collider::ColliderType::WALL:
				{
					if (owner1->entityType == Entity::EntityType::DYNAMIC_ENTITY)
					{
						DynamicEntity* dynOwner = (DynamicEntity*)owner1;
						if (dynOwner->dynamicType == DynamicEntity::DynamicType::PLAYER)	//Player vs Walls enabled
							return true;
						else
							return false;
					}
					break;
				}
				case Collider::ColliderType::PORTAL:
				{
					if (owner1->entityType == Entity::EntityType::DYNAMIC_ENTITY)
					{
						DynamicEntity* dynOwner = (DynamicEntity*)owner1;
						if (dynOwner->dynamicType == DynamicEntity::DynamicType::PLAYER)
							return true;
						else
							return false;
					}
					break;
				}
			}
		}
	}

	return false;
}

bool ModuleColliders::wereColliding(Collider* col1, Collider* col2) const
{
	std::list<Collider*>::iterator it;
	for (it = col1->colliding.begin(); it != col1->colliding.end(); ++it)
	{
		if ((*it) == col2)
			return true;
	}
	return false;
}

void ModuleColliders::PrintColliders() const
{
	std::list<Collider*>::const_iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		if ((*it)->owner == nullptr)
		{
			App->printer->PrintQuad((*it)->rectArea, { 255, 150, 255, 100 }, true, true); //We need to do this only when inside camera.
		}
		else
		{
			Entity* owner = (Entity*)(*it)->owner;
			SDL_Rect rect = { (*it)->rectArea.x + owner->pos.x, (*it)->rectArea.y + owner->pos.y, (*it)->rectArea.w, (*it)->rectArea.h };

			if ((*it)->colType == Collider::ColliderType::ENTITY)
			{	
				App->printer->PrintQuad(rect, { 255, 255, 255, 100 }, true, true);
			}
			else if ((*it)->colType == Collider::ColliderType::PLAYER_ATTACK)
			{
				App->printer->PrintQuad(rect, { 0, 255, 0, 100 }, true, true);
			}
			else if ((*it)->colType == Collider::ColliderType::ENEMY_ATTACK)
			{
				App->printer->PrintQuad(rect, { 255, 0, 0, 100 }, true, true);
			}
		}	
	}
}

void ModuleColliders::AddCommands()
{
	ConsoleOrder* order = new ConsoleColliders();
	App->console->AddConsoleOrderToList(order);
}