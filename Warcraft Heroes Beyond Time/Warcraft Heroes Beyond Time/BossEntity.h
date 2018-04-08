#ifndef __BOSSENTITY_H__
#define __BOSSENTITY_H__

#include "DynamicEntity.h"

class BossEntity : public DynamicEntity {
protected:
	enum class BossStates
	{
		NON_STATE = -1,
		IDLE
	} statesBoss = BossStates::NON_STATE;

public:
	BossEntity(fPoint coor, BOSS_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();

private:

	BOSS_TYPE type = BOSS_TYPE::NON_BOSS;
};

#endif