#include "BossEntity.h"
#include "Application.h"
#include "ModulePrinter.h"
#include "ModuleEntitySystem.h"

#include "ModuleTextures.h"
#include <list>

struct FelBall;

class Guldan : public BossEntity
{
private:

	bool createNewBalls = false;
	bool readeForTimeNewBalls = false;
	float timeForNewBalls = 0.0f;
	bool readyToTP = false;
	bool startTimeForTP = false;
	float floatTimeForTp = 0.0f;
	SDL_Texture* effectsTexture = nullptr;
	Animation idle, teleport, inverseTeleport;
	iPoint tpPoints[5] = { {14,4},{ 7,6 },{ 21,6 },{ 10,11 },{ 18,11 } };
	iPoint nextTpPos = {0,0};

	std::list<FelBall*> fellBallsList;
	
	enum class BossStates
	{
		NON_STATE = -1,
		IDLE,
		FEL_BALLS,
		TELEPORT,
		INVERSETELEPORT,
		DEAD,
		METEOR
	} statesBoss = BossStates::NON_STATE;

public:
	Guldan(fPoint coor, BOSS_TYPE type, SDL_Texture* texture);
	~Guldan();

	bool Start();
	bool Update(float dt);
	bool Finish();

	bool CreateFelBalls(fPoint pos);

};

struct FelBall
{
public:
	float ballSpeed = 400.0f;
	fPoint pos = { 0,0 };
	int startAngle = 0;
	int angleInside = 0;
	fPoint positionsToMove[360];
	float radius = 0.0f;
	int live = 200;
	SDL_Rect rect;
	Animation felAnim;
	bool startDying = false;
	bool startMovement = false;

	int circleIterator = 0;
	int contCirclesDone = 0;
	bool dead = false;

	float timeUntilRunAway = 0.0f;
	int rotation[360];
	int rotationCont = 0;

	SDL_Texture* tex = nullptr;
	Animation* anim = nullptr;

public:

	FelBall(fPoint pos, int radius, int angle, SDL_Texture* tex, int angleInside) : pos(pos), radius(radius), startAngle(angle), tex(tex), angleInside(angleInside)
	{
		felAnim.PushBack({ 19,32,18,23 });
		felAnim.PushBack({ 69,32,18,23 });
		felAnim.PushBack({ 118,32,18,23 });
		felAnim.PushBack({ 167,32,18,23 });
		felAnim.PushBack({ 216,32,18,23 });
		felAnim.PushBack({ 266,32,18,23 });
		felAnim.speedFactor = 9.0f;

		anim = &felAnim;
	}

	~FelBall() {};
	void StartCountDownToDie() { startDying = true; };
	void StartMovement() { 
		startMovement = true;

		float factor = (float)M_PI / 180.0f;
		int secondi = 0;
		for (uint i = 0; i < 360; ++i)
		{
			positionsToMove[i].x = (pos.x + radius * cos((i + startAngle) * factor)) + 20;
			positionsToMove[i].y = (pos.y + radius * sin((i + startAngle) * factor)) + 25;
			if (i > 359)
			{
				rotation[i] = angleInside + 1;
			}
			else
			{
				rotation[i] = secondi + 1;
			}
			rotation[i] = angleInside + i;
		}

		rotationCont = startAngle;
	};

	void Update(float dt)
	{
		if (startDying)
			live -= 1 * dt;

		if (live <= 0)
			dead = true;

		if (startMovement)
		{
			pos = positionsToMove[circleIterator++];

			if (circleIterator > 359)
			{
				circleIterator = 0;
				contCirclesDone++;
				if (contCirclesDone >= 1)
				{
					startMovement = false;
					StartCountDownToDie();
				}
			}
			rotationCont++;
			if (rotationCont > 359)
				rotationCont = 0;
		}
		else
		{
			if (timeUntilRunAway >= 1.0f)
			{
				if (startAngle == 0)
				{
					pos.x += ballSpeed * dt;
				}
				else if (startAngle == 45)
				{
					pos.x += ballSpeed/2 * dt;
					pos.y += ballSpeed/2 * dt;
				}
				else if (startAngle == 90)
				{
					pos.y += ballSpeed * dt;
				} 
				else if (startAngle == 135)
				{
					pos.x -= ballSpeed/2 * dt;
					pos.y += ballSpeed/2 * dt;
				}
				else if (startAngle == 180)
				{
					pos.x -= ballSpeed * dt;
				}
				else if (startAngle == 225)
				{
					pos.x -= ballSpeed/2 * dt;
					pos.y -= ballSpeed/2 * dt;
				}
				else if (startAngle == 270)
				{
					pos.y -= ballSpeed * dt;
				}
				else if (startAngle == 315)
				{
					pos.x += ballSpeed/2 * dt;
					pos.y -= ballSpeed/2 * dt;
				}

			}	 
			timeUntilRunAway += 1 * dt;
		}		 

		anim->speed = anim->speedFactor * dt;
	}
	void BlitFel()
	{
		App->printer->PrintSprite(iPoint((int)pos.x, (int)pos.y), tex, anim->GetCurrentFrame(), 1, ModulePrinter::Pivots::CENTER, rotation[rotationCont]);
	}
};

struct Meteor
{
	SDL_Rect rect;

	void Update() {};
};