#ifndef __DYNAMICENTITY_H__
#define __DYNAMICENTITY_H__

#include "Entity.h"

class DynamicEntity : public Entity 
{
public:
	enum class DynamicType
	{
		NO_TYPE = -1,
		ENEMY, 
		PLAYER
		
	} dynamicType = DynamicType::NO_TYPE;

	DynamicEntity(fPoint coor, SDL_Texture* texture, DynamicType dyntype);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();

	FIXED_ANGLE CaculateAngle(fPoint objectiveCoor);
	fPoint CaculateFPointAngle(fPoint objectiveCoor);
	uint DistanceToObejective(fPoint objectiveCoor);
	fPoint transformFixedAngleTofPoint(FIXED_ANGLE angle);

public:
	int movementSpeed = -1;
	int atacSpeed = -1;
	FIXED_ANGLE orientation = FIXED_ANGLE::NON_ANGLE;
};

#endif