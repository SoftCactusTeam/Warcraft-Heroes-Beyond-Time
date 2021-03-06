#ifndef __GULDAN_H__
#define __GULDAN_H__

#include "BossEntity.h"

#define GULDAN_BASE { 14 * 48 + 10,4 * 48 }
#define TIME_RESTORING_ENERGY 3.0f
#define TIME_BETWEEN_THUNDERS 0.2f
#define NUMBER_BALLS_ODD_EVEN 3
#define NUMBER_BALLS_COMPLETE_CIRCLE 3
#define NUMBER_BALLS_HEXAGON 10
#define NUMBER_BALLS_SPIRAL 20
#define RADIUS_BALLS 20
#define LIFE_BALLS 1000
#define TIME_BETWEEN_BALLS_ODD_EVEN 0.2f
#define TIME_BETWEEN_BALLS_COMPLETE_CIRCLE 1.8f
#define TIME_BETWEEN_BALLS_HEXAGON 0.5f
#define TIME_BETWEEN_BALLS_SPIRAL 0.1f
#define BOSS_CENTER { pos.x + 34, pos.y + 34 }
#define LAZINESS_LEVEL 2
#define BOSS_HP 3000.0f

#define OFFSET_TIME_SPIRAL_RECOVERY 5.0f

struct FelBall;
struct Collider;

class Guldan : public BossEntity
{
private:
	
	Animation idle, teleport, inverseTeleport, dead, startGeneratingBalls, generatingBalls, generatingBallsInverse, hello, restoreEnergy;

	Collider* guldanCollider = nullptr;
	Collider* wallGuldanCollider = nullptr;

	//BOSS STATE VARIABLE
	int randState = 0;

	// GENERATING BALLS VARIABLES
	int contBalls = 0;
	float timeBetweenBalls = 0.0f;
	float toAngle = 0.0f;

	// HEXAGON
	float hexagonAngle = 0.0f;
	bool startAfterVs = false;
	// SPIRAL
	float spiralAngle = 0.0f;
	float spiralRadiusIncreasement = 0.0f;
	float timeToComeBackSpiral = 0.0f;
	bool SpiralRecoveryOn = false;
	float timeOffset = 0.0f;

	// TELEPORT
	fPoint pointToTelerpot[3] = { { 14 * 48 + 10,7 * 48 },{ 12 * 48,10 * 48 },{ 17 * 48,10 * 48 } };
	bool teleportBase = false;
	bool teleportCenter = false;

	bool letsGoThunders = false;

	//RESTORING ENERGY
	float timeRestoring = 0.0f;

	// THUNDER
	int step = 0;
	float timeBetweenSteps = 0.0f;
	int randThunder = 0;

	// Geyser
	float timeBetweenGeyser = 0.0f;
	bool generateGeysers = false;
	float timeGeysersFollowingPlayerM = 0.0f;

	// ODD EVEN
	int repeat = 0;
	float timeBetweenM = 0.0f;
	bool startTimeBetweenM = false;

	// Tired
	int tired = 0;

	//AUDIOS
	bool play_this_audio_once = true;
	bool angry_sound_played_once = false;
	bool dead_sound_played_once = false;

	enum class FellBallsTypes
	{
		NO_TYPE,
		COMPLETE_CIRCLE,
		ODD_EVEN_TYPE,
		HEXAGON_TYPE,
		SPIRAL_TYPE
	} next_movement_type;

	enum class GeyserType
	{
		NO_TYPE,
		FOLLOW_PLAYER,
		STOP_IN_POS
	};

public:
	enum class BossStates
	{
		NON_STATE = -1,
		HELLO,
		IDLE,
		FEL_BALLS,
		TELEPORT,
		DEAD,
		GENERATINGBALLS,
		RESTORING_ENERGY,
		THUNDER_CAST
	} statesBoss = BossStates::NON_STATE;

public:
	Guldan(fPoint coor, BossType type, SDL_Texture* texture);
	~Guldan();
	
	bool Start();
	bool Update(float dt);
	bool Finish();

	void GenerateFelBalls(FellBallsTypes type, float angle) const;
	void GeneratGeyser(GeyserType type) const;
	void GenerateThunders(int numberXY);
	void GenerateInverseThunders(int numberXY);
	fPoint SetSpawnPointByAngle(fPoint pointToRotate, fPoint rotationPivot, double angle, double radius) const;
	float GetTimeToComeBackSpiral() const { return timeToComeBackSpiral; };

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);

	bool Draw();
};

#endif