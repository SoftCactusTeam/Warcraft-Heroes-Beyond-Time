#ifndef __DYNAMICENTITY_H__
#define __DYNAMICENTITY_H__

#include "Entity.h"

class DynamicEntity : public Entity {
public:
	DynamicEntity(fPoint coor, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();
	virtual void Collision(COLLIDER_TYPE type);

	FIXED_ANGLE CaculateAngle(fPoint objectiveCoor);
	fPoint CaculateFPointAngle(fPoint objectiveCoor);
	uint DistanceToObejective(fPoint objectiveCoor);

public:
	int live = -1;
	int movementSpeed = -1;
	int atacSpeed = -1;
	FIXED_ANGLE orientation = FIXED_ANGLE::NON_ANGLE;
};

#endif