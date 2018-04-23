#include "BossEntity.h"
#include "Application.h"
#include "ModulePrinter.h"
#include "ModuleEntitySystem.h"
#include "ModuleAudio.h"

#include "ModuleTextures.h"
#include <list>
#include <algorithm>

#include "ModuleColliders.h"

struct FelBall;

class Guldan : public BossEntity
{
private:
	
	Animation idle, teleport, inverseTeleport, dead, generateingBalls, generatingBallsInverse, hello;
	
	int hp = 0;
	
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

public:
	Guldan(fPoint coor, BossType type, SDL_Texture* texture);
	~Guldan();
	
	bool Start();
	bool Update(float dt);
	bool Finish();
};