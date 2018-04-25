#ifndef __GULDAN_H__
#define __GULDAN_H__

#include "BossEntity.h"

#define NUMBER_BALLS_ODD_EVEN 4
#define NUMBER_BALLS_COMPLETE_CIRCLE 1
#define NUMBER_BALLS_HEXAGON 36
#define NUMBER_BALLS_SPIRAL 20
#define RADIUS_BALLS 40
#define LIFE_BALLS 1000
#define TIME_BETWEEN_BALLS_ODD_EVEN 0.2f
#define TIME_BETWEEN_BALLS_COMPLETE_CIRCLE 0.2f
#define TIME_BETWEEN_BALLS_HEXAGON 0.2f
#define TIME_BETWEEN_BALLS_SPIRAL 0.1f
#define BOSS_CENTER { pos.x + 34, pos.y + 34 }

struct FelBall;

class Guldan : public BossEntity
{
private:
	
	Animation idle, teleport, inverseTeleport, dead, startGeneratingBalls, generatingBalls, generatingBallsInverse, hello;	
	int hp = 0;

	// GENERATING BALLS VARIABLES
	int contBalls = 0;
	float timeBetweenBalls = 0.0f;
	float toAngle = 0.0f;

	// HEXAGON
	float hexagonAngle = 0.0f;

	// SPIRAL
	float spiralAngle = 0.0f;
	float spiralRadiusIncreasement = 0.0f;
	float timeToComeBackSpiral = 0.0f;

	// TELEPORT
	fPoint pointToTelerpot[5] = { {0,0},{ 0,0 },{ 0,0 },{ 0,0 } };

	enum class BossStates
	{
		NON_STATE = -1,
		IDLE,
		FEL_BALLS,
		TELEPORT,
		INVERSETELEPORT,
		DEAD,
		GENERATINGBALLS,
	} statesBoss = BossStates::NON_STATE;

	enum class FellBallsTypes
	{
		NO_TYPE,
		COMPLETE_CIRCLE,
		ODD_EVEN_TYPE,
		HEXAGON_TYPE,
		SPIRAL_TYPE
	} next_movement_type;

public:
	Guldan(fPoint coor, BossType type, SDL_Texture* texture);
	~Guldan();
	
	bool Start();
	bool Update(float dt);
	bool Finish();

	void GenerateFelBalls(FellBallsTypes type, float angle) const;
	fPoint SetSpawnPointByAngle(fPoint pointToRotate, fPoint rotationPivot, double angle, double radius) const;
	float GetTimeToComeBackSpiral() const { return timeToComeBackSpiral; };

};

#endif