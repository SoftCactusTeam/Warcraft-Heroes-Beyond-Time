#ifndef __STATICENTITY_H__
#define __STATICENTITY_H__

#include "Entity.h"

class StaticEntity : public Entity {
public:
	StaticEntity(iPoint coor, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();
	virtual void Collision(COLLIDER_TYPE type);

};

#endif