#ifndef __Enemy_Archer_H__
#define __Enemy_Archer_H__

#include "EnemyEntity.h"
#include "ModuleColliders.h"
#include <vector>

#define ARROW_DEAD_TIMER 2000


enum ARCHER_STATE {
	ARCHER_IDLE,
	ARCHER_WALK,
	ARCHER_BASIC_ATAC,
	ARCHER_TRI_ATAC,
	ARCHER_FASTSHOOT_ATAC,
	ARCHER_BACKJUMP,
	ARCHER_LITTLEMOVE,
	ARCHER_DASH,
	ARCHER_SCAPE,
	ARCHER_FEAR,
	ARCHER_DIE
};

enum ARCHER_EFFECTS
{
	ARCHER_EFFECT_FREEZE,
	ARCHER_EFFECT_BURNING,
	ARCHER_EFFECT_NONE
};

struct archerEffectStruct
{
	ARCHER_EFFECTS effect = ARCHER_EFFECT_NONE;
	int time = 0;
};

class Enemy_Archer : public EnemyEntity
{
public:
	Enemy_Archer(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool Finish();
	bool Draw();
	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);

	// STATE MACHINE ====================

	void initIdle();
	void initWalk();
	void initAtac();
	void initTriAtac();
	void initFastAtac();
	void initBackJump();
	void initLittleMove();
	void initDash();
	void initDie();

	void doIdle();
	void doWalk();
	void doAtac();
	void doTriAtac();
	void doFastAtac();
	void doBackJump();
	void doLittleMove();
	void doDash();
	void doDie();

	void doFreeze(float dt);

	void Walk();
	void AddEffect(ARCHER_EFFECTS effect, int time);
	void UpdateEffects();
	// ~~~~~~~~~~~~~~~~~~ STATE MACHINE

	void LoadAnimations();
	void ShootArrow(fPoint desviation = fPoint(0, 0));

public:
	ARCHER_STATE state;
	Animation animIdle[NUMBER_OF_ORIENTATIONS];
	Animation animWalk[NUMBER_OF_ORIENTATIONS];
	Animation animAtac[NUMBER_OF_ORIENTATIONS];
	Animation animDeath[NUMBER_OF_ORIENTATIONS];
	Animation animFrozen[NUMBER_OF_ORIENTATIONS];

	Animation animSmoke;

	std::list<archerEffectStruct*> effectsList;

private:
	// Normal Atac Variables
	bool hasAttacked = false;
	// Fast atac variables
	int timeToShootAnother = 0;
	int arrowToShoot = 0;
	// TP variables
	int tempoSmoke = -1;
	fPoint posSmoke = { -1.f,-1.f };
	// Littlemove variables
	iPoint posToScape;
	//Items variables
	float fear_counter = 0.0f;

	int arrowsShooted = 0;
	int cooldownToReLittleMove = 0;
	// Dash variables
	FIXED_ANGLE saveFirstAngle = FIXED_ANGLE::NON_ANGLE;
	fPoint dashMovement;
	float dashTempo = 0.0f;

	//Items variables
	float frozen_counter = 0.0f;

	bool			damaged = false;
	float			damagedCD = 0.0f;

	int tier;
};

#endif
