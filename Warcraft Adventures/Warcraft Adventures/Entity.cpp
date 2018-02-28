#include "Entity.h"

FIXED_ANGLE DinamicEntity::CaculateAngle(iPoint objectiveCoor)
{
	FIXED_ANGLE angleToReturn;
	if (this->coor.x - objectiveCoor.x >= 0) {
		if (this->coor.y - objectiveCoor.y >= 0) {
			angleToReturn = FIXED_ANGLE::UP_LEFT;
		}
		else {
			angleToReturn = FIXED_ANGLE::DOWN_LEFT;
		}
	}
	else {
		if (this->coor.y - objectiveCoor.y >= 0) {
			angleToReturn = FIXED_ANGLE::UP_RIGHT;
		}
		else {
			angleToReturn = FIXED_ANGLE::DOWN_RIGHT;
		}
	}
}