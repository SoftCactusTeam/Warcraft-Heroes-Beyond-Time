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
			next_movement_type = FellBallsTypes::ODD_EVEN_TYPE;
			anim = &startGeneratingBalls;
			break;
		}

		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			statesBoss = BossStates::GENERATINGBALLS;
			next_movement_type = FellBallsTypes::COMPLETE_CIRCLE;
			anim = &startGeneratingBalls;
			break;
		}

		if (App->input->GetKey(SDL_SCANCODE_N))
		{
			statesBoss = BossStates::GENERATINGBALLS;
			next_movement_type = FellBallsTypes::HEXAGON_TYPE;
			anim = &startGeneratingBalls;
			break;
		}

		if (App->input->GetKey(SDL_SCANCODE_P))
		{
			statesBoss = BossStates::GENERATINGBALLS;
			next_movement_type = FellBallsTypes::SPIRAL_TYPE;
			timeToComeBackSpiral = 0.0f;
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

			if (next_movement_type == FellBallsTypes::ODD_EVEN_TYPE)
			{

				// ODD_EVEN_FEL_MOVEMENT -----
				if (timeBetweenBalls >= TIME_BETWEEN_BALLS_ODD_EVEN)
				{
					if (contBalls % 2 == 0)
						GenerateFelBalls(FellBallsTypes::ODD_EVEN_TYPE, 10.0f);
					else
						GenerateFelBalls(FellBallsTypes::ODD_EVEN_TYPE, -10.0f);

					timeBetweenBalls = 0.0f;
					contBalls += 1;
				}
				// ----------------------------

				if (contBalls >= NUMBER_BALLS_ODD_EVEN)
				{
					anim = &generatingBallsInverse;
					contBalls = 0;
				}
			}
			else if (next_movement_type == FellBallsTypes::COMPLETE_CIRCLE)
			{
				if (timeBetweenBalls >= TIME_BETWEEN_BALLS_COMPLETE_CIRCLE)
				{
					GenerateFelBalls(FellBallsTypes::COMPLETE_CIRCLE, 0.0f);
					timeBetweenBalls = 0.0f;
					contBalls += 1;
				}
				
				if (contBalls >= NUMBER_BALLS_COMPLETE_CIRCLE)
				{
					anim = &generatingBallsInverse;
					hexagonAngle = 0.0f;
					contBalls = 0;
				}
			}
			else if (next_movement_type == FellBallsTypes::HEXAGON_TYPE)
			{
				if (timeBetweenBalls >= TIME_BETWEEN_BALLS_HEXAGON)
				{
					GenerateFelBalls(FellBallsTypes::HEXAGON_TYPE, 0.0f);
					hexagonAngle += 10.0f;
					timeBetweenBalls = 0.0f;
					contBalls += 1;
				}
				// ----------------------------

				if (contBalls >= NUMBER_BALLS_HEXAGON)
				{
					anim = &generatingBallsInverse;
					hexagonAngle = 0.0f;
					contBalls = 0;
				}
			}
			else if (next_movement_type == FellBallsTypes::SPIRAL_TYPE)
			{
				if (timeBetweenBalls >= TIME_BETWEEN_BALLS_SPIRAL)
				{
					GenerateFelBalls(FellBallsTypes::SPIRAL_TYPE, 0.0f);
					spiralAngle += 10.0f;
					spiralRadiusIncreasement += 15.0f;
					timeBetweenBalls = 0.0f;
					contBalls += 1;
				}

				if (contBalls >= NUMBER_BALLS_SPIRAL)
				{
					anim = &generatingBallsInverse;
					spiralAngle = 0.0f;
					spiralRadiusIncreasement = 0.0f;
					contBalls = 0;
				}
				
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

	// spiral
	timeToComeBackSpiral += 1.0f * dt;

	anim->speed = anim->speedFactor * dt;

	return true;
}

bool Guldan::Finish()
{	
	return true;
}

void Guldan::GenerateFelBalls(FellBallsTypes type, float angleToIncrease) const
{
	FelBallInfo info;
	info.layer = 5;
	info.life = LIFE_BALLS,
	info.pos = BOSS_CENTER;
	info.rotationPivot = BOSS_CENTER;
	info.angle = angleToIncrease;

	fPoint defaultPoint = { pos.x + 34, pos.y + 34 - RADIUS_BALLS };

	switch (type)
	{
	case FellBallsTypes::COMPLETE_CIRCLE:

		info.radiusToIncrease = 150.0f;
		info.fel_movement = FelBallInfo::fel_ball_movement::complete_circle;

		for (int angle = 0; angle <= 360; angle += 12)
		{
			info.startRadius = RADIUS_BALLS;
			info.pos = SetSpawnPointByAngle(defaultPoint, BOSS_CENTER, angle, 40);
			App->projectiles->AddProjectile(&info, Projectile_type::fel_ball);
		}

		break;

	case FellBallsTypes::ODD_EVEN_TYPE:

		info.radiusToIncrease = 150.0f;
		info.fel_movement = FelBallInfo::fel_ball_movement::odd_even_type;

		for (int angle = 0; angle <= 360; angle += 45)
		{
			info.startRadius = RADIUS_BALLS;
			info.pos = SetSpawnPointByAngle(defaultPoint, BOSS_CENTER, angle, 40);
			App->projectiles->AddProjectile(&info, Projectile_type::fel_ball);

			info.pos = SetSpawnPointByAngle(defaultPoint, BOSS_CENTER, angle - 10.0f, 40);
			App->projectiles->AddProjectile(&info, Projectile_type::fel_ball);

			info.pos = SetSpawnPointByAngle(defaultPoint, BOSS_CENTER, angle + 10.0f, 40);
			App->projectiles->AddProjectile(&info, Projectile_type::fel_ball);
		}

		break;

	case FellBallsTypes::HEXAGON_TYPE:

		info.radiusToIncrease = 100.0f;
		info.fel_movement = FelBallInfo::fel_ball_movement::hexagon;

		for (int angle = 0; angle <= 360; angle += 60)
		{
			info.startRadius = RADIUS_BALLS;
			info.pos = SetSpawnPointByAngle(defaultPoint, BOSS_CENTER, angle + hexagonAngle, 40);
			App->projectiles->AddProjectile(&info, Projectile_type::fel_ball);
		}

		break;

	case FellBallsTypes::SPIRAL_TYPE:

		info.radiusToIncrease = 0.0f;
		info.fel_movement = FelBallInfo::fel_ball_movement::spiral;

		for (int angle = 0; angle <= 360; angle += 60)
		{
			info.startRadius = RADIUS_BALLS + spiralRadiusIncreasement;
			info.pos = SetSpawnPointByAngle(defaultPoint, BOSS_CENTER, angle + spiralAngle, 40);
			App->projectiles->AddProjectile(&info, Projectile_type::fel_ball);
		}

		break;
	}
}

fPoint Guldan::SetSpawnPointByAngle(fPoint pointToRotate, fPoint rotationPivot, double angle, double radius) const
{
	angle = DEG_2_RAD(angle);

	fPoint toReturn;

	fPoint difference(pointToRotate.x - rotationPivot.x, pointToRotate.y - rotationPivot.y);

	difference.x *= radius;
	difference.y *= radius;

	int norm = pointToRotate.DistanceTo(rotationPivot);

	difference.x /= norm;
	difference.y /= norm;

	toReturn.x = cos(angle) * difference.x - sin(angle) * difference.y;
	toReturn.y = sin(angle) * difference.x + cos(angle) * difference.y;

	return fPoint(toReturn.x + rotationPivot.x, toReturn.y + rotationPivot.y);
}
