#include "BossEntity.h"

#define NUMBER_BALLS 100
#define RADIUS_BALLS 40
#define LIFE_BALLS 100
#define TIME_BETWEEN_BALLS 0.4f
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
		TOTAL_COS_SIN,
		PARCIAL_COS_SIN,
		BOTH_TOTAL_PARCIAL
	};

public:
	Guldan(fPoint coor, BossType type, SDL_Texture* texture);
	~Guldan();
	
	bool Start();
	bool Update(float dt);
	bool Finish();

	void GenerateFelBalls(FellBallsTypes type, float angle) const;
	fPoint SetSpawnPointByAngle(fPoint pointToRotate, fPoint rotationPivot, double angle, double radius) const;

};