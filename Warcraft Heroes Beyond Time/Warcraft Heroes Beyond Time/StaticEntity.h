#ifndef __STATICENTITY_H__
#define __STATICENTITY_H__

#include "Entity.h"

class Collider;

class StaticEntity : public Entity {
public:
	enum class StaticType
	{
		NO_TYPE,
		CHEST,
		PORTAL
	} staticType = StaticType::NO_TYPE;

	StaticEntity(fPoint coor, SDL_Texture* texture, StaticType staticType = StaticType::NO_TYPE);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();
};

#endif