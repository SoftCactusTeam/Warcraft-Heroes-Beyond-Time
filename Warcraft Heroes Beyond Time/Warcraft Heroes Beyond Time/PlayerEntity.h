#ifndef __PLAYERENTITY_H__
#define __PLAYERENTITY_H__

#include "DynamicEntity.h"
#include "Item.h"
#include "ModuleEntitySystem.h"
#include "ModuleAudio.h"


#include <list>

class Collider;
struct SDL_Texture;

class PlayerEntity : public DynamicEntity
{
public:

	Animation idleDown, idleUp, idleLeft, idleRight, idleUpRight, idleUpLeft, idleDownRight, idleDownLeft;
	Animation up, down, left, right, upLeft, upRight, downLeft, downRight;
	Animation dashRight, dashDown, dashUpRight, dashDownRight, dashDownLeft, dashUp, dashLeft, dashUpLeft;
	Animation attackDown, attackDownLeft, attackDownRight, attackUp, attackUpRight, attackUpLeft, attackLeft, attackRight;
	Animation skill;
	Animation deadUpRight, deadDownRight;

protected:

	PLAYER_TYPE type = PLAYER_TYPE::NON_PLAYER;
	Animation* animBefore = nullptr;

	float speed = 250.0f;
	bool move = true;
	bool damaged = false;

	float damagedConfigCD = 0.0f; //This will be the one loaded from config.xml
	float damagedCD = 0.0f;

	float DashConfigCD = 0.0f;
	float DashCD = 0.0f;

	float deadinfloorConfigCD = 0.0f;
	float deadinfloorcd = 0.0f;

	bool win = false;
	float afterWinConfigCounter = 0.0f; //Wont be used for now
	float afterWinCounter = 0.0f;

	Collider* wallCol = nullptr;
	Collider* damageCol = nullptr;

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
		PL_WIN

	} state;

public:

	PlayerEntity(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	EntitySystem::PlayerStats numStats;

	void Walk(bool);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool PostUpdate() { return true; }
	bool Draw();
	void PlayerStates(float dt);
	void KeyboardStates(float dt);
	void JoyconStates(float dt);
	bool getConcretePlayerStates(int stat);
	void CheckIddleStates();

	void CheckMapLimits();

	virtual bool Finish();
	
	Collider* GetDamageCollider() const;

	//Stats functions
	void SetDamage(int damage, bool setStateDamage);
	void IncreaseEnergy(int percent);

	//This function calculates player position given a Bezier Curve
	fPoint CalculatePosFromBezier(fPoint startPos, fPoint handleA, float t, fPoint handleB, fPoint endPos);

	Animation* GetAnimFromAngle(float angle, bool dashOn = false);
	FIXED_ANGLE returnFixedAngle();
	bool IsPlayerMoving();

	// Bezier/dash related variables
	fPoint handleA = { 0.4f, 0.35f };
	fPoint handleB = { 0.5f, 0.8f };
	fPoint endPos = { 0.0f, 0.0f };
	float angle = 0.0f;
	float dashDistance = 100.0f;
	float t = 0.0f;
	fPoint startPos = { 0.0f, 0.0f };
	void ResetDash();
	


	//Camera culling
	SDL_Rect freeZone;
	float freeZonex, freeZoney;
	void InitCulling();
	void CheckCulling();
	bool drawFZ = false;
	void DrawFreeZone(bool);

	//Collisions
	virtual void UpdateCollider() {}
	void PushOut(Collider* wall);
	virtual void Attack() {}
	virtual void UseSkill() {}

	//Win
	void Win()
	{
		App->audio->PlayMusic(App->audio->WinBSO.data(), 0.5);
		win = true;
		state = states::PL_WIN;
		anim = &idleDown;
	}

	
};

#endif