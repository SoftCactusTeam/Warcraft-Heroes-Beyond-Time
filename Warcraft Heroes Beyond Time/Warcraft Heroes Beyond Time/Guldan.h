#include "BossEntity.h"

#define NUMBER_BALLS 20
#define RADIUS_BALLS 40
#define LIFE_BALLS 100000
#define TIME_BETWEEN_BALLS 0.5f
struct FelBall;

class Guldan : public BossEntity
{
private:
	
	Animation idle, teleport, inverseTeleport, dead, startGeneratingBalls, generatingBalls, generatingBallsInverse, hello;
	
	int hp = 0;


	// GENERATING BALLS VARIABLES
	int contBalls = 0;
	float timeBetweenBalls = 0.0f;

	
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
		PARCIAL_COS_SIN
	};

public:
	Guldan(fPoint coor, BossType type, SDL_Texture* texture);
	~Guldan();
	
	bool Start();
	bool Update(float dt);
	bool Finish();

	void GenerateFelBalls(FellBallsTypes type) const;
};