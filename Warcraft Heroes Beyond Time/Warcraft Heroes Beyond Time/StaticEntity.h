#ifndef __STATICENTITY_H__
#define __STATICENTITY_H__

#include "Entity.h"

class Collider;

class StaticEntity : public Entity {
public:
	StaticEntity(fPoint coor, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();
};

#endif