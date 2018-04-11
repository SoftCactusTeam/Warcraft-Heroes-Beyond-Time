#ifndef __PLAYERENTITY_H__
#define __PLAYERENTITY_H__

#include "DynamicEntity.h"
#include "Item.h"
#include "ModuleEntitySystem.h"


#include <list>

class Collider;

class PlayerEntity : public DynamicEntity 
{

protected:

	PLAYER_TYPE type = PLAYER_TYPE::NON_PLAYER;
	Animation idleDown, idleUp, idleLeft, idleRight, idleUpRight, idleUpLeft, idleDownRight, idleDownLeft;
	Animation up, down, left, right, upLeft, upRight, downLeft, downRight;
	Animation dashRight, dashDown, dashUpRight, dashDownRight, dashDownLeft, dashUp, dashLeft, dashUpLeft;
	Animation attackDown, attackUp, attackLeft, attackRight;
	Animation skill;
	Animation damagedAnim, dead;
	Animation* animBefore = nullptr;
	float speed = 250.0f;
	bool move = true;
	bool damaged = false;

	std::list<Item> itemsActive;

	enum class states
	{
		PL_NON_STATE,
		PL_IDLE,
		PL_UP,
		PL_DOWN,
		PL_LEFT,
		PL_RIGHT,
		PL_UP_RIGHT,
		PL_UP_LEFT,
		PL_DOWN_RIGHT,
		PL_DOWN_LEFT,
		PL_MOVE,
		PL_DASH,
		PL_ATTACK,
		PL_SKILL,
		PL_DEAD,
		PL_DAMAGE

	} state;

	Collider* pcol = nullptr;


public:

	PlayerEntity(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	EntitySystem::PlayerStats numStats;

	void Walk(bool);

	virtual bool Start();
	virtual bool Update(float dt);
	void PlayerStates(float dt);
	void KeyboardStates(float dt);
	void JoyconStates(float dt);

	void CheckIddleStates();

	void CheckMapLimits();

	virtual bool Finish();
	virtual void Collision(COLLIDER_TYPE type);

	void setCol(Collider* pcol);

	void AddItem(Item item);
	void IterateItems(ItemFunctions nameFunction);
	void SetDamage(int damage, bool setStateDamage);

	//This function calculates player position given a Bezier Curve
	fPoint CalculatePosFromBezier(fPoint startPos, fPoint handleA, float t, fPoint handleB, fPoint endPos);
	
	Animation* GetAnimFromAngle(float angle, bool dashOn = false);
	bool IsPlayerMoving();

	// Bezier/dash related variables
	fPoint handleA = { 0.6f, 0.0f };
	fPoint handleB = { 0.4f, 1.0f };
	fPoint endPos = { 0.0f, 0.0f };
	float angle = 0.0f;
	float dashDistance = 150.0f;
	float t = 0.0f;
	fPoint startPos = { 0.0f, 0.0f };

	//Camera culling
	SDL_Rect freeZone;
	float freeZonex, freeZoney;
	void InitCulling();
	void CheckCulling();
	bool drawFZ = false;
	void DrawFreeZone(bool);
	
};

#endif