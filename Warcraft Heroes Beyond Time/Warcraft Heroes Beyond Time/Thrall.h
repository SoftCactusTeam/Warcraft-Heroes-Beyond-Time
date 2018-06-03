#ifndef __THRALL_H__
#define __THRALL_H__

#include "PlayerEntity.h"
#include "EffectsElem.h"
#include "ModuleEntitySystem.h"

class Collider;

class Thrall : public PlayerEntity {
private:
public:
	Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture, EntitySystem::PlayerStats& numStats = EntitySystem::PlayerStats());

	bool Update(float dt);
	bool PostUpdate();
	bool Finish();
	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);

private:
	void UpdateCollider();
	void UpdateAttackCollider();
	void Attack();
	void UseSkill();
	void UpdateSkillCollider();

private:
	Collider * attackCollider = nullptr;
	Collider* skillCollider = nullptr;
	bool attacking = false;
	bool skillOn = false;
	uint hit_state = 0; //See thrall.cpp collider player attack
	enum HIT_STATE
	{
		HIT_1 = 1,
		HIT_2,
		HIT_3,
		MAX_HITS
	};
	uint attack_thrall_state = 0;
	enum ATTACK_STATE
	{
		ATTACK_STATE_1 = 1,
		ATTACK_STATE_2,
		ATTACK_STATE_3,
		MAX_ATTACKS
	};
};

#endif