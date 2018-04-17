#ifndef __BOSSENTITY_H__
#define __BOSSENTITY_H__

#include "EnemyEntity.h"

enum class BossType
{
	NO_TYPE = -1,
	GULDAN

};

class BossEntity : public EnemyEntity  
{
public:
	

	BossEntity(fPoint coor, BossType type, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();
	
	BossType bossType = BossType::NO_TYPE;
};

#endif