#ifndef __CHESTENTITY_H__
#define __CHESTENTITY_H__

#include "StaticEntity.h"

class ChestEntity : public StaticEntity {
public:
	ChestEntity(fPoint coor, CHEST_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();
	bool OpenChest(); // it must return an item
	void UnLockChest();
	inline bool IsLocked() const;

private:
	CHEST_TYPE type = CHEST_TYPE::NON_CHEST;
	Animation chest;
	bool locked = true;
};

#endif