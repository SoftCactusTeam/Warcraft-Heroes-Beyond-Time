#include "BossEntity.h"
#include "Application.h"
#include "ModulePrinter.h"
#include "ModuleEntitySystem.h"

#include <list>

struct FelBall;

class Guldan : public BossEntity
{
private:

	Animation idle;
	iPoint tpPoints[5] = { {15,5},{ 8,7 },{ 22,7 },{ 11,12 },{ 19,12 } };
	iPoint nextTpPos = {0,0};

	std::list<FelBall*> fellBallsList;
	
	enum class BossStates
	{
		NON_STATE = -1,
		IDLE,
		FEL_BALLS,
		TELEPORT,
		DEAD,
		METEOR
	} statesBoss = BossStates::NON_STATE;

public:
	Guldan(fPoint coor, BOSS_TYPE type, SDL_Texture* texture);
	~Guldan();

	bool Start();
	bool Update(float dt);
	bool Finish();

	bool CreateFelBall(fPoint pos);

};

struct FelBall
{
public:
	fPoint pos = { 0,0 };
	int startAngle = 0;
	fPoint positionsToMove[360];
	float radius = 0.0f;
	int live = 100;
	SDL_Rect rect;
	Animation felAnim;
	bool startDying = false;
	bool startMovement = false;

	int circleIterator = 0;
	int contCirclesDone = 0;

	float timeUntilRunAway = 0.0f;
	int rotation = 0;

public:

	FelBall(fPoint pos, int radius, int angle) : pos(pos), radius(radius), startAngle(angle)
	{
		felAnim.PushBack({ 0,0,89,71 });
	}
	~FelBall() {};
	void StartCountDownToDie() { startDying = true; };
	void StartMovement() { 
		startMovement = true;

		float factor = (float)M_PI / 180.0f;

		for (uint i = 0; i < 360; ++i)
		{
			positionsToMove[i].x = (pos.x + radius * cos((i + startAngle) * factor)) + 23;
			positionsToMove[i].y = (pos.y + radius * sin((i + startAngle) * factor)) + 23;
		}
	};
	void Update(float dt)
	{
		if (startDying)
			live -= 1 * dt;

		if (live <= 0)
			this->~FelBall();

		if (startMovement)
		{
			pos = positionsToMove[circleIterator++];

			if (circleIterator > 359)
			{
				circleIterator = 0;
				contCirclesDone++;
				if (contCirclesDone >= 2)
				{
					startMovement = false;
				}
			}
			rotation++;
		}
		else
		{
			if (timeUntilRunAway >= 2.0f)
			{
				float factor = (float)M_PI / 180.0f;

				pos.x = pos.x + radius * cos(startAngle * factor);
				pos.y = pos.y + radius * sin(startAngle * factor);

				radius += 0.1f * dt;

				rotation++;
			}
			timeUntilRunAway += 1 * dt;
		}
	}
	void BlitFel()
	{
		App->printer->PrintSprite(iPoint((int)pos.x, (int)pos.y), App->entities->spritesheetsEntities[THRALL_SHEET], { 684,1112,23,23 }, 1, ModulePrinter::Pivots::CENTER, rotation);
	}
};

struct Meteor
{
	SDL_Rect rect;

	void Update() {};
};