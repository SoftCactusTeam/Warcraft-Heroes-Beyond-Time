#ifndef __CHESTENTITY_H__
#define __CHESTENTITY_H__

#include "StaticEntity.h"

class Collider;

class ChestEntity : public StaticEntity {
public:
	ChestEntity(fPoint coor, CHEST_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();

	void OnCollision(Collider* yours, Collider* collideWith);

	bool OpenChest();


private:
	CHEST_TYPE type = CHEST_TYPE::NON_CHEST;
	Animation chest;
	bool opened = false;
	bool selection_created = false;
	Collider* col = nullptr;

};

#endif