#include "BossEntity.h"

class Guldan : public BossEntity
{

	Guldan(fPoint coor, BOSS_TYPE type, SDL_Texture* texture);
	~Guldan();

	bool Start();
	bool Update(float dt);
	bool Finish();

};