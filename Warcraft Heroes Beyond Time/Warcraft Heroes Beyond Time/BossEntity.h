#ifndef __BOSSENTITY_H__
#define __BOSSENTITY_H__

#include "EnemyEntity.h"

class BossEntity : public EnemyEntity  
{
public:
	BossEntity(fPoint coor, BOSS_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();

private:

	BOSS_TYPE type = BOSS_TYPE::NON_BOSS;
};

#endif