#ifndef __ModuleColliders_H__
#define __ModuleColliders_H__

#include "Module.h"
#include <list>
#include "SDL\include\SDL.h"
#include <memory>

class Collider
{
public:
	enum class ColliderType
	{
		NO_TYPE = -1,
		WALL,
		PORTAL,
		ENTITY,
		BOSS,
		PLAYER_ATTACK,
		ENEMY_ATTACK

	} colType = ColliderType::NO_TYPE;

	void* owner = nullptr;
	SDL_Rect rectArea = { 0,0,0,0 };
	std::list<Collider*> colliding;

public:

	Collider(SDL_Rect& rectArea, ColliderType colType, void* owner = nullptr) : rectArea(rectArea), colType(colType), owner(owner) {}
	virtual ~Collider()
	{
		colliding.clear();
	}
};

class PlayerAttack : public Collider
{
public:
	enum class P_Attack_Type
	{
		NO_TYPE = -1,
		NORMAL_ATTACK,
		DMGBALL_ITEM,
		FREEZEBALL_ITEM,
		FEARBALL_ITEM,
		SHIELDBALL_ITEM,

		SKILL,
		DAMAGESHIT_ITEM,
		PROJECTILESLOWSHIT_ITEM,
		ENEMYSLOWSHIT_ITEM

	} pattacktype = P_Attack_Type::NO_TYPE;

	float damage = 0.0f;

	PlayerAttack(SDL_Rect& rectArea, ColliderType colType, void* owner, float damage, P_Attack_Type pattacktype) : Collider(rectArea, colType, owner), damage(damage), pattacktype(pattacktype) {}
	virtual ~PlayerAttack() {}
};

class EnemyAttack : public Collider
{
public:
	enum class E_Attack_Type
	{
		NO_TYPE = -1,
		ARROW,
		GULDAN_BALL

	} eattacktype = E_Attack_Type::NO_TYPE;

	float damage = 0.0f;

	EnemyAttack(SDL_Rect& rectArea, ColliderType colType, void* owner, float damage, E_Attack_Type eattacktype) : Collider(rectArea, colType, owner), damage(damage), eattacktype(eattacktype) {}
	virtual ~EnemyAttack() {}
};

class ModuleColliders : public Module
{
public:

	ModuleColliders();

	void Init()
	{
		active = false;
	}

	bool Awake(pugi::xml_node& consoleNode);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void AddCommands();

	std::weak_ptr<Collider*> AddCollider(SDL_Rect rectArea, Collider::ColliderType colType, void* owner = nullptr);
	std::weak_ptr<Collider*> AddPlayerAttackCollider(SDL_Rect rectArea, void* owner, float damage, PlayerAttack::P_Attack_Type pattacktype);
	std::weak_ptr<Collider*> AddEnemyAttackCollider(SDL_Rect rectArea, void* owner, float damage, EnemyAttack::E_Attack_Type eattacktype);

	void deleteCollider(Collider* col);
	void deleteColliderbyOwner(void* owner);

	void PrintColliders() const;

	bool collideWithWalls(SDL_Rect newPos, SDL_Rect& otherCol);

private:

	bool AreNearEnough(Collider* col1, Collider* col2) const;
	bool CheckIfCollides(Collider* col1, Collider* col2) const;
	bool CollisionEnabled(Collider* col1, Collider* col2) const;
	bool wereColliding(Collider* col1, Collider* col2) const;

private:
	std::list<std::shared_ptr<Collider*>> colliderList;


public:
	bool printColliders = false;

};

#endif
