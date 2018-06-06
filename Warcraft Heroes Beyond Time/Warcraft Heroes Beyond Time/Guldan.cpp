#include "Guldan.h"
#include "Application.h"
#include "ModuleProjectiles.h"
#include "FelBall.h"
#include "Geyser.h"
#include "Thunder.h"
#include "Block.h"
#include "Scene.h"
#include "PlayerEntity.h"

#include "ModuleInput.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"
#include "ModuleMapGenerator.h"
#include "ModuleGUI.h"


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

	startGeneratingBalls.PushBack({ 416,1,56,68 });
	startGeneratingBalls.PushBack({ 485,1,56,68 });
	startGeneratingBalls.PushBack({ 554,1,56,68 });
	startGeneratingBalls.PushBack({ 623,1,56,68 });
	startGeneratingBalls.PushBack({ 692,1,56,68 });
	startGeneratingBalls.PushBack({ 761,1,56,68 });
	generatingBalls.PushBack({ 830,1,56,68 });
	generatingBalls.PushBack({ 899,1,56,68 });
	generatingBalls.PushBack({ 2,70,56,68 });
	generatingBalls.PushBack({ 71,70,56,68 });
	startGeneratingBalls.loop = false;
	generatingBalls.speedFactor = 9.0f;
	startGeneratingBalls.speedFactor = 9.0f;

	generatingBallsInverse.PushBack({ 899,1,56,68 });
	generatingBallsInverse.PushBack({ 830,1,56,68 });
	generatingBallsInverse.PushBack({ 761,1,56,68 });
	generatingBallsInverse.PushBack({ 692,1,56,68 });
	generatingBallsInverse.PushBack({ 623,1,56,68 });
	generatingBallsInverse.PushBack({ 554,1,56,68 });
	generatingBallsInverse.PushBack({ 485,1,56,68 });
	generatingBallsInverse.PushBack({ 416,1,56,68 });
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

	restoreEnergy.PushBack({ 209,345,65,68 });
	restoreEnergy.PushBack({ 278,345,65,68 });
	restoreEnergy.PushBack({ 348,345,65,68 });
	restoreEnergy.PushBack({ 415,345,65,68 });
	restoreEnergy.speedFactor = 9.0f;

	anim = &idle;

	numStats = App->entities->guldanstats;

	numStats.hp = numStats.maxhp = BOSS_HP;

	isGuldan = true;
}

Guldan::~Guldan()
{
}

bool Guldan::Start()
{
	statesBoss = BossStates::HELLO;

	guldanCollider = *App->colliders->AddCollider({ 0,0,68,68 }, Collider::ColliderType::ENTITY, this).lock();

	wallGuldanCollider = *App->colliders->AddCollider({ (int)pos.x, (int)pos.y, 68,68}, Collider::ColliderType::WALL, nullptr).lock();

	//SOUNDS RESTARTED FROM THE BEGINING OF GULDAN
	angry_sound_played_once = false;
	dead_sound_played_once = false;

	return true;
}

bool Guldan::Update(float dt)
{
	switch (statesBoss)
	{
	case BossStates::HELLO:

		if (pos.DistanceTo(App->scene->player->pos) <= 150.0f && !App->gui->finishedVs)
		{
			App->gui->startVs = true;
			BlockInfo info;
			info.layer = 1;
			info.pos = { 14 * 48 - 1, 13 * 48 };
			App->projectiles->AddProjectile(&info, Projectile_type::block);
			info.pos = { 15 * 48 - 2, 13 * 48 };
			App->projectiles->AddProjectile(&info, Projectile_type::block);
			info.pos = { 16 * 48 - 4, 13 * 48 };
			App->projectiles->AddProjectile(&info, Projectile_type::block);
			App->scene->player->Walk(false);

			if (play_this_audio_once)
			{
				App->audio->PlayFx(App->audio->G_vs_T_fx);
				play_this_audio_once = false;
			}
		}

		if (App->gui->finishedVs)
		{
			App->scene->player->Walk(true);
			App->input->PlayJoyRumble(0.9f, 100);
			anim = &hello;
			//ENCOUNTER AUDIO
			if (!play_this_audio_once)
			{
				App->audio->PlayFx(App->audio->GuldanEncounterFX);
				play_this_audio_once = true;
			}
		}

		if (anim == &hello && anim->Finished())
		{
			anim = &idle;
			statesBoss = BossStates::TELEPORT;
			anim = &teleport;
			teleportCenter = true;
			App->audio->PlayFx(App->audio->GuldanTPFX);
			App->gui->CreateBossHPBar((BossEntity*)this, { 640 / 2 - 312 / 2,320 });
			play_this_audio_once = true;
			App->gui->finishedVs = false;
			break;
		}

		break;

	case BossStates::IDLE:
	{

		if (numStats.hp <= 0)
		{
			anim = &dead;
			statesBoss = BossStates::DEAD;
			break;
		}

		if (SpiralRecoveryOn)
		{
			timeOffset += 1.0f * dt;
			if (timeOffset >= OFFSET_TIME_SPIRAL_RECOVERY)
			{
				timeOffset = 0.0f;
				SpiralRecoveryOn = false;
			}
			break;
		}


		if (!letsGoThunders && pos.x == (14 * 48 + 10) && pos.y == (4 * 48))
		{
			statesBoss = BossStates::TELEPORT;
			anim = &teleport;
			App->audio->PlayFx(App->audio->GuldanTPFX);
			break;
		}

		if (letsGoThunders)
		{
			statesBoss = BossStates::THUNDER_CAST;
			randThunder = rand() % 2;
			anim = &startGeneratingBalls;
			letsGoThunders = false;
			break;
		}

		if (tired >= LAZINESS_LEVEL)
		{
			anim = &restoreEnergy;
			statesBoss = BossStates::RESTORING_ENERGY;
			tired = 0;
			break;
		}

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			GeneratGeyser(GeyserType::FOLLOW_PLAYER);
			break;
		}

		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			GeneratGeyser(GeyserType::STOP_IN_POS);
			break;
		}
		/*if (numStats.hp <= numStats.maxhp / 2)
			randState = rand() % 6;
		else
			randState = rand() % 5;	*/

		if (randState == 0)
		{
			statesBoss = BossStates::TELEPORT;
			anim = &teleport;
			App->audio->PlayFx(App->audio->GuldanTPFX);
			randState = 2;
			break;
		}

		if (randState == 1)
		{
			statesBoss = BossStates::TELEPORT;
			teleportBase = true;
			letsGoThunders = true;
			anim = &teleport;
			tired += 1;
			App->audio->PlayFx(App->audio->GuldanTPFX);
			randState = 4;
			break;
		}

		if (randState == 2)
		{
			statesBoss = BossStates::GENERATINGBALLS;
			next_movement_type = FellBallsTypes::ODD_EVEN_TYPE;
			anim = &startGeneratingBalls;
			tired += 1;
			App->audio->PlayFx(App->audio->GuldanOddBallFX);
			randState = 3;
			break;
		}

		if (randState == 3)
		{
			statesBoss = BossStates::GENERATINGBALLS;
			next_movement_type = FellBallsTypes::COMPLETE_CIRCLE;
			anim = &startGeneratingBalls;
			tired += 1;
			randState = 1;
			break;
		}

		if (randState == 4)
		{
			statesBoss = BossStates::GENERATINGBALLS;
			next_movement_type = FellBallsTypes::HEXAGON_TYPE;
			anim = &startGeneratingBalls;
			tired += 1;
			if ((numStats.hp <= numStats.maxhp / 2))
				randState = 5;
			else
				randState = 0;
			break;
		}

		if (randState == 5)
		{
			statesBoss = BossStates::GENERATINGBALLS;
			next_movement_type = FellBallsTypes::SPIRAL_TYPE;
			SpiralRecoveryOn = true;
			timeToComeBackSpiral = 0.0f;
			anim = &startGeneratingBalls;
			tired += 1;
			App->audio->PlayFx(App->audio->GuldanSpiralFX);
			randState = 0;
			break;
		}

		break;
	}
	case BossStates::GENERATINGBALLS:
		
		if (numStats.hp <= 0)
		{
			anim = &dead;
			statesBoss = BossStates::DEAD;
			break;
		}

		if (anim == &startGeneratingBalls && anim->Finished())
			anim = &generatingBalls;

		else if (anim == &generatingBalls)
		{
			timeBetweenBalls += 1.0f * dt;

				if (numStats.hp <= numStats.maxhp / 2 && next_movement_type != FellBallsTypes::SPIRAL_TYPE)
				{
					timeGeysersFollowingPlayerM += 1 * dt;

					if (timeGeysersFollowingPlayerM >= 3.0f)
					{
						GeneratGeyser(GeyserType::FOLLOW_PLAYER);
						App->audio->PlayFx(App->audio->GuldanMeteoriteTarget);
						timeGeysersFollowingPlayerM = 0.0f;
					}
				}

			if (next_movement_type == FellBallsTypes::ODD_EVEN_TYPE)
			{
				if (startTimeBetweenM)
				{
					timeBetweenM += 1.0f * dt;
				}
				// ODD_EVEN_FEL_MOVEMENT -----
				if ((startTimeBetweenM && timeBetweenM >= 1.4f) || !startTimeBetweenM)
				{
					if (timeBetweenBalls >= TIME_BETWEEN_BALLS_ODD_EVEN)
					{
						if (contBalls % 2 == 0)
							GenerateFelBalls(FellBallsTypes::ODD_EVEN_TYPE, 10.0f);
						else
							GenerateFelBalls(FellBallsTypes::ODD_EVEN_TYPE, -10.0f);

						timeBetweenBalls = 0.0f;
						contBalls += 1;
					}
					startTimeBetweenM = false;
					timeBetweenM = 0.0f;
				}
				// ----------------------------

				if (contBalls >= NUMBER_BALLS_ODD_EVEN)
				{
					contBalls = 0;
					repeat += 1;
					startTimeBetweenM = true;
				}

				if (repeat >= 1)
				{
					anim = &generatingBallsInverse;
					repeat = 0;
				}
			}
			else if (next_movement_type == FellBallsTypes::COMPLETE_CIRCLE)
			{
				if (play_this_audio_once) 
				{
					App->audio->PlayFx(App->audio->GuldanCircle);
					play_this_audio_once = false;
				}

				if (timeBetweenBalls >= TIME_BETWEEN_BALLS_COMPLETE_CIRCLE)
				{
					GenerateFelBalls(FellBallsTypes::COMPLETE_CIRCLE, 0.0f);
					timeBetweenBalls = 0.0f;
					contBalls += 1;
					play_this_audio_once = true;
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
					App->audio->PlayFx(App->audio->GuldanHexBall);
				}
				// ----------------------------

				if (contBalls >= NUMBER_BALLS_HEXAGON)
				{

					next_movement_type = FellBallsTypes::COMPLETE_CIRCLE;
					//anim = &generatingBallsInverse;
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

	case BossStates::TELEPORT:

		if (anim == &teleport)
		{
			if (anim->Finished())
			{
				anim->Reset();

				if (!teleportBase && !teleportCenter)
				{
					int posToTp = 0;
					do
						posToTp = rand() % 3;
					while (pointToTelerpot[posToTp] == pos);
					pos.x = pointToTelerpot[posToTp].x;
					pos.y = pointToTelerpot[posToTp].y;
				}
				else if (teleportCenter && !teleportBase)
				{
					pos.x = pointToTelerpot[0].x;
					pos.y = pointToTelerpot[0].y;
				}
				else if (!teleportCenter && teleportBase)
					pos = GULDAN_BASE;

					anim = &inverseTeleport;
					App->audio->PlayFx(App->audio->GuldanTPFX);
			}
		}
		else
		{
			if (anim->Finished())
			{
				anim->Reset();
				anim = &idle;
				statesBoss = BossStates::IDLE;
				teleportBase = false;
				teleportCenter = false;
				break;
			}
		}
			break;

	case BossStates::THUNDER_CAST:
	{
		if (play_this_audio_once)
		{
			App->audio->PlayFx(App->audio->GuldanThunderFX);
			//App->audio->PlayFx(App->audio->GuldanTPFX);
			play_this_audio_once = false;
		}

		timeBetweenSteps += 1.0f * dt;

		if (timeBetweenSteps >= TIME_BETWEEN_THUNDERS)
		{
			timeBetweenSteps = 0.0f;
			if (randThunder == 0)
				GenerateThunders(step);
			else
				GenerateInverseThunders(step);
			step += 1;
		}

		if (step >= 29)
		{
			step = 0;
			statesBoss = BossStates::IDLE;
			anim = &idle;
			play_this_audio_once = true;
			break;
		}

		break;
	}

	case BossStates::RESTORING_ENERGY:

		if (numStats.hp <= 0)
		{
			anim = &dead;
			statesBoss = BossStates::DEAD;
			break;
		}

		if (timeRestoring == 0.0f)
		{
			App->audio->PlayFx(App->audio->GuldanChargingPower);
		}

		timeRestoring += 1 * dt;

		if (timeRestoring >= TIME_RESTORING_ENERGY)
		{
			timeRestoring = 0.0f;
			anim = &idle;
			statesBoss = BossStates::IDLE;
		}

		break;

	case BossStates::DEAD:
		anim = &dead;
		if (!dead_sound_played_once)
		{
			App->audio->PlayFx(App->audio->GuldanDieFX);
			dead_sound_played_once = true;
		}

		if (anim->Finished())
		{
			App->audio->HaltFX(App->audio->GuldanFireSecondPhase);
			App->scene->player->Win();
		}

		break;
	}

	if (numStats.hp <= (numStats.maxhp / 2) && statesBoss != BossStates::DEAD)
	{
		timeBetweenGeyser += 1 * dt;

		if (timeBetweenGeyser >= 0.8f)
		{
			timeBetweenGeyser = 0.0f;
			GeneratGeyser(GeyserType::STOP_IN_POS);

			//SECOND PHASE SOUNDS
			if (numStats.hp <= numStats.maxhp / 2 && !angry_sound_played_once)
			{
				App->audio->PlayFx(App->audio->GuldanSecondPhase);
				App->audio->PlayFx(App->audio->GuldanFireSecondPhase, -1);
				angry_sound_played_once = true;
			}
		}
	}

	// spiral
	timeToComeBackSpiral += 1.0f * dt;

	anim->speed = anim->speedFactor * dt;

	wallGuldanCollider->rectArea = { (int)pos.x,(int)pos.y,68,68 };

	return true;
}

bool Guldan::Finish()
{	
	App->colliders->deleteCollider(guldanCollider);
	App->colliders->deleteCollider(wallGuldanCollider);
	guldanCollider = nullptr;
	wallGuldanCollider = nullptr;

	return true;
}

void Guldan::GenerateFelBalls(FellBallsTypes type, float angleToIncrease) const
{
	FelBallInfo info;
	info.layer = 1;
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

void Guldan::GeneratGeyser(GeyserType type) const
{
	GeyserInfo info;
	info.pos = { 0,0 };
	info.layer = 5;

	switch (type)
	{
	case GeyserType::FOLLOW_PLAYER:
		info.geyser_movement = GeyserInfo::Geyser_style::follow_player;
		break;

	case GeyserType::STOP_IN_POS:
		iPoint aux_point = App->map->GetRandomBossPoint();
		info.pos = { (float)aux_point.x * 48, (float)aux_point.y * 48 };
		info.geyser_movement = GeyserInfo::Geyser_style::stay_in_pos;
		break;
	}

	App->projectiles->AddProjectile(&info, Projectile_type::geyser);
}

void Guldan::GenerateThunders(int numberXY)
{
	ThunderInfo info;
	info.pos = { 10.0f,5.0f };

	info.layer = 10;

	if (numberXY == 1)
	{
		info.pos = { 11.0f,8.0f };
	}
	else if (numberXY == 2)
	{
		info.pos = { 12.0f,7.0f };
	}
	else if (numberXY == 3)
	{
		info.pos = { 11.0f,10.0f };
	}
	else if (numberXY == 4)
	{
		info.pos = { 12.0f,9.0f };
	}
	else if (numberXY == 5)
	{
		info.pos = { 13.0f,8.0f };
	}
	else if (numberXY == 6)
	{
		info.pos = { 14.0f,7.0f };
	}
	else if (numberXY == 7)
	{
		info.pos = { 11.0f,12.0f };
	}
	else if (numberXY == 8)
	{
		info.pos = { 12.0f,11.0f };
	}
	else if (numberXY == 9)
	{
		info.pos = { 13.0f,10.0f };
	}
	else if (numberXY == 10)
	{
		info.pos = { 14.0f,9.0f };
	}
	else if (numberXY == 11)
	{
		info.pos = { 15.0f,8.0f };
	}
	else if (numberXY == 12)
	{
		info.pos = { 16.0f,7.0f };
	}
	else if (numberXY == 13)
	{
		info.pos = { 13.0f,12.0f };
	}
	else if (numberXY == 14)
	{
		info.pos = { 14.0f,11.0f };
	}
	else if (numberXY == 15)
	{
		info.pos = { 15.0f,10.0f };
	}
	else if (numberXY == 16)
	{
		info.pos = { 16.0f,9.0f };
	}
	else if (numberXY == 17)
	{
		info.pos = { 17.0f,8.0f };
	}
	else if (numberXY == 20)
	{
		info.pos = { 18.0f,7.0f };
	}
	else if (numberXY == 21)
	{
		info.pos = { 15.0f,12.0f };
	}
	else if (numberXY == 22)
	{
		info.pos = { 16.0f,11.0f };
	}
	else if (numberXY == 23)
	{
		info.pos = { 17.0f,10.0f };
	}
	else if (numberXY == 24)
	{
		info.pos = { 18.0f,9.0f };
	}
	else if (numberXY == 25)
	{
		info.pos = { 19.0f,8.0f };
	}
	else if (numberXY == 26)
	{
		info.pos = { 17.0f,12.0f };
	}
	else if (numberXY == 27)
	{
		info.pos = { 18.0f,11.0f };
	}
	else if (numberXY == 28)
	{
		info.pos = { 19.0f,10.0f };
	}
	else if (numberXY == 29)
	{
		info.pos = { 19.0f,12.0f };
	}
	else
		return;

	info.pos.x *= 48.0f;
	info.pos.y *= 48.0f;

	App->projectiles->AddProjectile(&info, Projectile_type::thunder);
}

void Guldan::GenerateInverseThunders(int numberXY)
{
	ThunderInfo info;
	info.pos = { 10.0f,5.0f };

	info.layer = 10;

	
	if (numberXY == 1)
	{
		info.pos = { 18.0f,7.0f };
	}
	else if (numberXY == 2)
	{
		info.pos = { 19.0f,8.0f };
	}
	else if (numberXY == 3)
	{
		info.pos = { 16.0f,7.0f };
	}
	else if (numberXY == 4)
	{
		info.pos = { 17.0f,8.0f };
	}
	else if (numberXY == 5)
	{
		info.pos = { 18.0f,9.0f };
	}
	else if (numberXY == 6)
	{
		info.pos = { 19.0f,10.0f };
	}
	else if (numberXY == 7)
	{
		info.pos = { 14.0f,7.0f };
	}
	else if (numberXY == 8)
	{
		info.pos = { 15.0f,8.0f };
	}
	else if (numberXY == 9)
	{
		info.pos = { 16.0f,9.0f };
	}
	else if (numberXY == 10)
	{
		info.pos = { 17.0f,10.0f };
	}
	else if (numberXY == 11)
	{
		info.pos = { 18.0f,11.0f };
	}
	else if (numberXY == 12)
	{
		info.pos = { 19.0f,12.0f };
	}
	else if (numberXY == 13)
	{
		info.pos = { 12.0f,7.0f };
	}
	else if (numberXY == 14)
	{
		info.pos = { 13.0f,8.0f };
	}
	else if (numberXY == 15)
	{
		info.pos = { 14.0f,9.0f };
	}
	else if (numberXY == 16)
	{
		info.pos = { 15.0f,10.0f };
	}
	else if (numberXY == 17)
	{
		info.pos = { 16.0f,11.0f };
	}
	else if (numberXY == 18)
	{
		info.pos = { 17.0f,12.0f };
	}
	else if (numberXY == 19)
	{
		info.pos = { 11.0f,8.0f };
	}
	else if (numberXY == 20)
	{
		info.pos = { 12.0f,9.0f };
	}
	else if (numberXY == 21)
	{
		info.pos = { 13.0f,10.0f };
	}
	else if (numberXY == 22)
	{
		info.pos = { 14.0f,11.0f };
	}
	else if (numberXY == 23)
	{
		info.pos = { 15.0f,12.0f };
	}
	else if (numberXY == 24)
	{
		info.pos = { 11.0f,10.0f };
	}
	else if (numberXY == 25)
	{
		info.pos = { 12.0f,11.0f };
	}
	else if (numberXY == 26)
	{
		info.pos = { 13.0f,12.0f };
	}
	else if (numberXY == 27)
	{
		info.pos = { 9.0f,10.0f };
	}
	else if (numberXY == 28)
	{
		info.pos = { 11.0f,12.0f };
	}
	else
		return;

	info.pos.x *= 48.0f;
	info.pos.y *= 48.0f;

	App->projectiles->AddProjectile(&info, Projectile_type::thunder);
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

void Guldan::OnCollision(Collider* yours, Collider* collideWith)
{
	switch (collideWith->colType)
	{
		case Collider::ColliderType::PLAYER_ATTACK:
		{
			PlayerAttack* attack = (PlayerAttack*)collideWith;
		
			if (anim != &teleport || anim != &inverseTeleport)
			{
				numStats.hp -= attack->damage;
				if (numStats.hp <= 0.0f)
				{
					numStats.hp = 0.0f;
					statesBoss = BossStates::DEAD;
				}	
			}
			break;
		}	
	}
}

void Guldan::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
	switch (collideWith->colType)
	{
	case Collider::ColliderType::PLAYER_ATTACK:
	{
		PlayerAttack* attack = (PlayerAttack*)collideWith;
		if (attack->pattacktype == PlayerAttack::P_Attack_Type::DAMAGESHIT_ITEM)
		{
			if (anim != &teleport || anim != &inverseTeleport)
			{
				numStats.hp -= attack->damage;
				if (numStats.hp <= 0.0f)
				{
					numStats.hp = 0.0f;
					statesBoss = BossStates::DEAD;
				}
			}
		}

		break;
	}
	}
}

bool Guldan::Draw()
{
	bool ret = true;
	if (pos.x == (14 * 48 + 10) && pos.y == (4 * 48))
		ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), texture, anim->GetCurrentFrame(), 5, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot());
	else
		ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), texture, anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot());

	return ret;
}
