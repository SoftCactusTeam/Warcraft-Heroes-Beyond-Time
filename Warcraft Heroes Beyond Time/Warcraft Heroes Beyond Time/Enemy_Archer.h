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
	ARCHER_SCAPE
};

class Enemy_Archer_Arrow {
public:
	Enemy_Archer_Arrow(fPoint coor, SDL_Texture* texture, fPoint direction, int deadTimer = ARROW_DEAD_TIMER);

	void Update();
	void Finish();

public:
	fPoint			pos;
	fPoint			direction;
	SDL_Texture*	texture;
	SDL_Rect		rect;
	int				deadTimer;
	bool			destroy = false;
	Collider*		arrowCollider = nullptr;
	float angle;
};

class Enemy_Archer : public EnemyEntity
{
public:
	Enemy_Archer(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool Finish();

	void initIdle();
	void initWalk();
	void initAtac();
	void initTriAtac();
	void initFastAtac();
	void initBackJump();
	void initScape();

	void doIdle();
	void doWalk();
	void doAtac();
	void doTriAtac();
	void doFastAtac();
	void doBackJump();
	void doScape();

	void ChargeAnimations();
	void ShootArrow(fPoint desviation = fPoint(0,0));

public:
	ARCHER_STATE state;
	Animation animIdle[NUMBER_OF_ORIENTATIONS];
	Animation animWalk[NUMBER_OF_ORIENTATIONS];
	Animation animAtac[NUMBER_OF_ORIENTATIONS];
	Animation animDeath[NUMBER_OF_ORIENTATIONS];

	Animation animSmoke;

	std::vector<Enemy_Archer_Arrow*> arrowsVector;

private:
	// Fast atac variables
	int timeToShootAnother = 0;
	int arrowToShoot = 0;
	// TP variables
	int tempoSmoke = -1;
	fPoint posSmoke = { -1.f,-1.f };
	// Scape variables
	iPoint posToScape;
};

#endif