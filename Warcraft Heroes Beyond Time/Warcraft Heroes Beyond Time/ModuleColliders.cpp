#include "ModuleColliders.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Console.h"
#include "Entity.h"
#include "DynamicEntity.h"
#include "ModulePrinter.h"
#include "ModuleProjectiles.h"
#include "ModuleMapGenerator.h"
#include "Projectile.h"

#include "Brofiler\Brofiler.h"


#include "PlayerEntity.h"
#include "StaticEntity.h"




static const float MINIMAL_DISTANCE_TO_COLLIDE = 200;


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

	std::list<std::shared_ptr<Collider*>>::iterator col1;
	std::list<std::shared_ptr<Collider*>>::iterator col2;

	for (col1 = colliderList.begin(); col1 != colliderList.end(); ++col1)
	{
		Collider* collider1 = **col1;
		if (collider1->colType == Collider::ColliderType::WALL)
			continue;

		for (col2 = colliderList.begin(); col2 != colliderList.end(); ++col2)
		{
			Collider* collider2 = **col2;
			if (collider1 == collider2 || !CollisionEnabled(collider1, collider2) || !AreNearEnough(collider1, collider2))
				continue;

			if (CheckIfCollides(collider1, collider2))			//If collides
			{
				if (collider1->owner != nullptr)
				{
					if (collider1->colType != Collider::ColliderType::ENEMY_ATTACK)
					{
						Entity* owner = (Entity*)collider1->owner;
						if (wereColliding(collider1, collider2))
							owner->OnCollisionContinue(collider1, collider2);
						else
						{
							owner->OnCollision(collider1, collider2);
							collider1->colliding.push_back(collider2);
						}
					}
					else
					{
						Projectile* owner = (Projectile*)collider1->owner;
						if (wereColliding(collider1, collider2))
							owner->OnCollisionContinue(collider1, collider2);
						else
						{
							owner->OnCollision(collider1, collider2);
							collider1->colliding.push_back(collider2);
						}
					}
				}

				if (collider2->owner != nullptr)
				{
					if (collider2->colType != Collider::ColliderType::ENEMY_ATTACK)
					{
						Entity* owner = (Entity*)collider2->owner;
						if (wereColliding(collider2, collider1))
							owner->OnCollisionContinue(collider2, collider1);
						else
						{
							owner->OnCollision(collider2, collider1);
							collider2->colliding.push_back(collider1);
						}
					}
					else
					{
						Projectile* owner = (Projectile*)collider2->owner;
						if (wereColliding(collider2, collider1))
							owner->OnCollisionContinue(collider2, collider1);
						else
						{
							owner->OnCollision(collider2, collider1);
							collider2->colliding.push_back(collider1);
						}
					}
				}
			}
			else                                                //If don't collide			
			{
				if (wereColliding(collider1, collider2))				//If were colliding
				{
					if (collider1->owner != nullptr)
					{
						if (collider1->colType != Collider::ColliderType::ENEMY_ATTACK)
						{
							Entity* owner = (Entity*)collider1->owner;
							owner->OnCollisionLeave(collider1, collider2);
							collider1->colliding.remove(collider2);
						}
						else
						{
							Projectile* owner = (Projectile*)collider1->owner;
							owner->OnCollisionLeave(collider1, collider2);
							collider1->colliding.remove(collider2);
						}
					}
				}

				if (wereColliding(collider2, collider1))
				{
					if (collider2->owner != nullptr)
					{
						if (collider2->colType != Collider::ColliderType::ENEMY_ATTACK)
						{
							Entity* owner = (Entity*)collider2->owner;
							owner->OnCollisionLeave(collider2, collider1);
							collider2->colliding.remove(collider1);
						}
						else
						{
							Projectile* owner = (Projectile*)collider2->owner;
							owner->OnCollisionLeave(collider2, collider1);
							collider2->colliding.remove(collider1);
						}
					}

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
	std::list<std::shared_ptr<Collider*>>::iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		(*it).reset();
	}
	colliderList.clear();

	return true;
}

std::weak_ptr<Collider*> ModuleColliders::AddCollider(SDL_Rect rectArea, Collider::ColliderType colType, void* owner)
{
	Collider* collider = new Collider(rectArea, colType, owner);
	std::shared_ptr<Collider*> shared = std::make_shared<Collider*>(collider);
	colliderList.push_back(shared);

	return std::weak_ptr<Collider*>(shared);
}

std::weak_ptr<Collider*> ModuleColliders::AddPlayerAttackCollider(SDL_Rect rectArea, void* owner, float damage, PlayerAttack::P_Attack_Type pattacktype)
{
	PlayerAttack* playerattack = new PlayerAttack(rectArea, Collider::ColliderType::PLAYER_ATTACK, owner, damage, pattacktype);
	std::shared_ptr<Collider*> shared = std::make_shared<Collider*>(playerattack);
	colliderList.push_back(shared);

	return std::weak_ptr<Collider*>(shared);
}

std::weak_ptr<Collider*> ModuleColliders::AddEnemyAttackCollider(SDL_Rect rectArea, void* owner, float damage, EnemyAttack::E_Attack_Type eattacktype)
{
	EnemyAttack* enemyattack = new EnemyAttack(rectArea, Collider::ColliderType::ENEMY_ATTACK, owner, damage, eattacktype);
	std::shared_ptr<Collider*> shared = std::make_shared<Collider*>(enemyattack);
	colliderList.push_back(shared);

	return std::weak_ptr<Collider*>(shared);
}

void ModuleColliders::deleteCollider(Collider* col)
{
	std::list<std::shared_ptr<Collider*>>::iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		if ((*(*it)) == col)
		{
			(*it).reset();
			colliderList.erase(it);
			break;
		}
	}
}

void ModuleColliders::deleteColliderbyOwner(void* owner)
{
	std::list<std::shared_ptr<Collider*>>::iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		if ((*(*it))->owner == owner)
		{
			(*it).reset();
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
			case Collider::ColliderType::ENEMY_ATTACK:
			{
				Projectile* owner = (Projectile*)col1->owner;
				float x, y;
				owner->getPos(x, y);
				rect1.x += x;
				rect1.y += y;
				break;
			}
			default:
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
			case Collider::ColliderType::ENEMY_ATTACK:
			{
				Projectile* owner = (Projectile*)col2->owner;
				float x, y;
				owner->getPos(x, y);
				rect2.x += x;
				rect2.y += y;
				break;
			}
			default:
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

bool ModuleColliders::AreNearEnough(Collider* col1, Collider* col2) const
{
	bool ret = false;

	iPoint pos1 = { col1->rectArea.x, col1->rectArea.y };

	if (col1->owner != nullptr)
	{
		switch (col1->colType)
		{
			case Collider::ColliderType::ENEMY_ATTACK:
			{
				Projectile* owner = (Projectile*)col1->owner;
				float x, y;
				owner->getPos(x, y);
				pos1.x += x;
				pos1.y += y;
				break;
			}
			default:
			{
				Entity* owner = (Entity*)col1->owner;
				pos1.x += owner->pos.x;
				pos1.y += owner->pos.y;
				break;
			}
		}
	}

	iPoint pos2 = { col2->rectArea.x, col2->rectArea.y };
	if (col2->owner != nullptr)
	{
		switch (col2->colType)
		{
			case Collider::ColliderType::ENEMY_ATTACK:
			{
				Projectile* owner = (Projectile*)col2->owner;
				float x, y;
				owner->getPos(x, y);
				pos2.x += x;
				pos2.y += y;
				break;
			}
			default:
			{
				Entity* owner = (Entity*)col2->owner;
				pos2.x += owner->pos.x;
				pos2.y += owner->pos.y;
				break;
			}
		}
	}


	return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2)) <= MINIMAL_DISTANCE_TO_COLLIDE;
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
					else if (owner1->entityType == Entity::EntityType::STATIC_ENTITY)
					{
						StaticEntity* statOwner = (StaticEntity*)owner1;
						if (statOwner->staticType == StaticEntity::StaticType::CHEST)
							return true;													//Chest vs Player Attack enabled
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
		case Collider::ColliderType::ENEMY_ATTACK:
		{
			if (col2->colType == Collider::ColliderType::WALL || col2->colType == Collider::ColliderType::PLAYER_ATTACK)  //Proyectiles vs walls and player attacks enabled
				return true;										
			else
				return false;
			break;
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
	std::list<std::shared_ptr<Collider*>>::const_iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		if ((*(*it))->owner == nullptr)
		{
			int tileSize = App->map->getTileSize();

			if ((*(*it))->rectArea.x >= (-1 * App->render->camera.x) - tileSize &&
				(*(*it))->rectArea.y >= (-1 * App->render->camera.y) - tileSize &&
				(*(*it))->rectArea.x + (*(*it))->rectArea.w < -App->render->camera.x + App->render->camera.w + tileSize &&
				(*(*it))->rectArea.y + (*(*it))->rectArea.h < -App->render->camera.y + App->render->camera.h + tileSize)
				
					App->printer->PrintQuad((*(*it))->rectArea, { 255, 150, 255, 100 }, true, true);
		}
		else
		{
			if ((*(*it))->colType != Collider::ColliderType::ENEMY_ATTACK)
			{
				Entity* owner = (Entity*)(*(*it))->owner;
				SDL_Rect rect = { (*(*it))->rectArea.x + owner->pos.x, (*(*it))->rectArea.y + owner->pos.y, (*(*it))->rectArea.w, (*(*it))->rectArea.h };

				if ((*(*it))->colType == Collider::ColliderType::ENTITY)
				{
					App->printer->PrintQuad(rect, { 255, 255, 255, 100 }, true, true);
				}
				else if ((*(*it))->colType == Collider::ColliderType::PLAYER_ATTACK)
				{
					App->printer->PrintQuad(rect, { 0, 255, 0, 100 }, true, true);
				}
			}
			else
			{
				if ((*(*it))->colType == Collider::ColliderType::ENEMY_ATTACK)
				{
					Projectile* owner = (Projectile*)(*(*it))->owner;
					float x, y;
					owner->getPos(x, y);
					SDL_Rect rect = { (*(*it))->rectArea.x + x, (*(*it))->rectArea.y + y, (*(*it))->rectArea.w, (*(*it))->rectArea.h };
					App->printer->PrintQuad(rect, { 255, 0, 0, 100 }, true, true);
				}
			}
			
		}	
	}
}

bool ModuleColliders::collideWithWalls(SDL_Rect newPos, SDL_Rect& otherCol)
{
	std::list<std::shared_ptr<Collider*>>::iterator it;
	for (it = colliderList.begin(); it != colliderList.end(); ++it)
	{
		Collider* col = **it;
		if (col->colType != Collider::ColliderType::WALL)
			continue;
		
		SDL_Rect temp = col->rectArea;
		otherCol = temp;
		
		if (newPos.x < otherCol.x + otherCol.w && newPos.x + newPos.w > otherCol.x &&
			newPos.y < otherCol.y + otherCol.h && newPos.y + newPos.h > otherCol.y)
			return true;
	}
	return false;
}

void ModuleColliders::AddCommands()
{
	ConsoleOrder* order = new ConsoleColliders();
	App->console->AddConsoleOrderToList(order);
}