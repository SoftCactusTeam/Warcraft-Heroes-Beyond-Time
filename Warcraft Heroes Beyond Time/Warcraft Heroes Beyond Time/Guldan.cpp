#include "Guldan.h"
#include "Application.h"
#include "ModuleProjectiles.h"
#include "FelBall.h"

#include "ModuleInput.h"

Guldan::Guldan(fPoint coor, BossType type, SDL_Texture* texture) : BossEntity(coor, type, texture)
{
	idle.PushBack({ 1,1,68,68 });
	idle.PushBack({ 70,1,68,68 });
	idle.PushBack({ 139,1,68,68 });
	idle.PushBack({ 208,1,68,68 });
	idle.PushBack({ 277,1,68,68 });
	idle.speedFactor = 9.0f;

	teleport.PushBack({ 484,70 ,68,68 });
	teleport.PushBack({ 553,70 ,68,68 });
	teleport.PushBack({ 622,70 ,68,68 });
	teleport.PushBack({ 691,70 ,68,68 });
	teleport.PushBack({ 760,70 ,68,68 });
	teleport.PushBack({ 829,70 ,68,68 });
	teleport.PushBack({ 898,70 ,68,68 });
	teleport.PushBack({ 1  ,139,68,68 });
	teleport.PushBack({ 70 ,139,68,68 });
	teleport.PushBack({ 139,139,68,68 });
	teleport.PushBack({ 208,139,68,68 });
	teleport.PushBack({ 277,139,68,68 });
	teleport.PushBack({ 346,139,68,68 });
	teleport.PushBack({ 415,139,68,68 });
	teleport.PushBack({ 484,139,68,68 });
	teleport.PushBack({ 553,139,68,68 });
	teleport.PushBack({ 622,139,68,68 });
	teleport.PushBack({ 691,139,68,68 });
	teleport.PushBack({ 760,139,68,68 });
	teleport.speedFactor = 9.0f;
	teleport.loop = false;

	inverseTeleport.PushBack({ 760,139,68,68 });
	inverseTeleport.PushBack({ 691,139,68,68 });
	inverseTeleport.PushBack({ 622,139,68,68 });
	inverseTeleport.PushBack({ 553,139,68,68 });
	inverseTeleport.PushBack({ 484,139,68,68 });
	inverseTeleport.PushBack({ 415,139,68,68 });
	inverseTeleport.PushBack({ 346,139,68,68 });
	inverseTeleport.PushBack({ 277,139,68,68 });
	inverseTeleport.PushBack({ 208,139,68,68 });
	inverseTeleport.PushBack({ 139,139,68,68 });
	inverseTeleport.PushBack({ 70 ,139,68,68 });
	inverseTeleport.PushBack({ 1  ,139,68,68 });
	inverseTeleport.PushBack({ 898,70 ,68,68 });
	inverseTeleport.PushBack({ 829,70 ,68,68 });
	inverseTeleport.PushBack({ 760,70 ,68,68 });
	inverseTeleport.PushBack({ 691,70 ,68,68 });
	inverseTeleport.PushBack({ 622,70 ,68,68 });
	inverseTeleport.PushBack({ 553,70 ,68,68 });
	inverseTeleport.PushBack({ 484,70 ,68,68 });
	inverseTeleport.speedFactor = 9.0f;
	inverseTeleport.loop = false;

	dead.PushBack({ 2,283,60,64 });
	dead.PushBack({ 71,283,60,64 });
	dead.PushBack({ 140,283,60,64 });
	dead.PushBack({ 209,283,60,64 });
	dead.PushBack({ 278,283,60,64 });
	dead.PushBack({ 347,283,60,64 });
	dead.PushBack({ 416,283,60,64 });
	dead.PushBack({ 487,283,60,64 });
	dead.PushBack({ 555,283,60,64 });
	dead.PushBack({ 624,283,60,64 });
	dead.PushBack({ 691,283,60,64 });
	dead.PushBack({ 760,283,60,64 });
	dead.PushBack({ 829,283,60,64 });
	dead.PushBack({ 898,283,60,64 });
	dead.loop = false;
	dead.speedFactor = 9.0f;

	startGeneratingBalls.PushBack({ 484,1,68,68 });
	startGeneratingBalls.PushBack({ 553,1,68,68 });
	startGeneratingBalls.PushBack({ 623,1,68,68 });
	startGeneratingBalls.PushBack({ 692,1,68,68 });
	startGeneratingBalls.PushBack({ 760,1,68,68 });
	startGeneratingBalls.PushBack({ 830,1,68,68 });
	startGeneratingBalls.PushBack({ 898,1,68,68 });
	generatingBalls.PushBack({ 830,1,68,68 });
	generatingBalls.PushBack({ 898,1,68,68 });
	startGeneratingBalls.loop = false;
	generatingBalls.speedFactor = 9.0f;
	startGeneratingBalls.speedFactor = 9.0f;

	generatingBallsInverse.PushBack({ 898,1,68,68 });
	generatingBallsInverse.PushBack({ 830,1,68,68 });
	generatingBallsInverse.PushBack({ 898,1,68,68 });
	generatingBallsInverse.PushBack({ 830,1,68,68 });
	generatingBallsInverse.PushBack({ 760,1,68,68 });
	generatingBallsInverse.PushBack({ 692,1,68,68 });
	generatingBallsInverse.PushBack({ 623,1,68,68 });
	generatingBallsInverse.PushBack({ 553,1,68,68 });
	generatingBallsInverse.PushBack({ 484,1,68,68 });
	generatingBallsInverse.loop = false;
	generatingBallsInverse.speedFactor = 9.0f;

	hello.PushBack({ 208,71,69,68 });
	hello.PushBack({ 276,71,68,68 });
	hello.PushBack({ 346,71,68,68 });
	hello.PushBack({ 415,71,68,68 });
	hello.PushBack({ 346,71,68,68 });
	hello.PushBack({ 276,71,68,68 });
	hello.PushBack({ 208,71,68,68 });
	hello.loop = false;
	hello.speedFactor = 9.0f;

	anim = &idle;

	numStats = App->entities->guldanstats;

	isGuldan = true;
}

Guldan::~Guldan()
{
}

bool Guldan::Start()
{
	statesBoss = BossStates::IDLE;

	return true;
}

bool Guldan::Update(float dt)
{
	switch (statesBoss)
	{
	case BossStates::IDLE:
			
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			statesBoss = BossStates::GENERATINGBALLS;
			anim = &startGeneratingBalls;
			break;
		}

		break;

	case BossStates::GENERATINGBALLS:
		
		if (anim == &startGeneratingBalls && anim->Finished())
			anim = &generatingBalls;

		else if (anim == &generatingBalls)
		{
			timeBetweenBalls += 1.0f * dt;

			if (timeBetweenBalls >= TIME_BETWEEN_BALLS)
			{ 
				GenerateFelBalls(FellBallsTypes::TOTAL_COS_SIN);
				timeBetweenBalls = 0.0f;
				contBalls += 1;
			}

			if (contBalls >= NUMBER_BALLS)
			{
				anim = &generatingBallsInverse;
				contBalls = 0;
			}
		}

		else
		{
			if (generatingBallsInverse.Finished())
			{
				startGeneratingBalls.Reset();
				generatingBalls.Reset();
				generatingBallsInverse.Reset();
				anim = &idle;
				statesBoss = BossStates::IDLE;
			}
		}
		break;
	}



	anim->speed = anim->speedFactor * dt;

	return true;
}

bool Guldan::Finish()
{	
	return true;
}

void Guldan::GenerateFelBalls(FellBallsTypes type) const
{
	FelBallInfo info;
	info.layer = 5;
	info.life = LIFE_BALLS;
	info.pos = { pos.x + 34 - 7, pos.y + 34 };
	info.speed = 0.0f;
	info.rotationPivot = { pos.x + 34 - 7,pos.y + 34 };
	info.angle = 0.0f;
	info.radiusToIncrease = 4.0f;

	switch (type)
	{
	case FellBallsTypes::TOTAL_COS_SIN:
	{
		int numBalls = 0;

		while (numBalls != 4)
		{
			if (numBalls == 0)
				info.pos = { pos.x + 34 - 7 + RADIUS_BALLS, pos.y + 34 };
			else if (numBalls == 1)
				info.pos = { pos.x + 34 - 7 - RADIUS_BALLS, pos.y + 34 };
			else if (numBalls == 2)
				info.pos = { pos.x + 34 - 7, pos.y + 34 + RADIUS_BALLS };
			else if (numBalls == 3)
				info.pos = { pos.x + 34 - 7, pos.y + 34 - RADIUS_BALLS };

			App->projectiles->AddProjectile(info, Projectile_type::fel_ball);

			numBalls++;
		}

		break;
	}

	case FellBallsTypes::PARCIAL_COS_SIN:

		int numBalls = 0;

		while (numBalls != 4)
		{
			if (numBalls == 0)
				info.pos = { pos.x + 34 - 7 + RADIUS_BALLS, pos.y + 34 };
			else if (numBalls == 1)
				info.pos = { pos.x + 34 - 7 - RADIUS_BALLS, pos.y + 34 };
			else if (numBalls == 2)
				info.pos = { pos.x + 34 - 7, pos.y + 34 + RADIUS_BALLS };
			else if (numBalls == 3)
				info.pos = { pos.x + 34 - 7, pos.y + 34 - RADIUS_BALLS };

			App->projectiles->AddProjectile(info, Projectile_type::fel_ball);

			numBalls++;
		}

		break;

	}
}
