#ifndef __PORTALENTITY_H__
#define __PORTALENTITY_H__

#include "StaticEntity.h"

class Collider;

class PortalEntity : public StaticEntity {
public:
	PortalEntity(fPoint coor, STATIC_ENTITY_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();
	void OpenPortal();
	bool PlayerNear(fPoint pos);

	bool Draw();

	Collider* portalCol=nullptr;
	bool locked = true;
private:
	STATIC_ENTITY_TYPE type = STATIC_ENTITY_TYPE::NON_SENTITY;
	Animation portal;
	
};

#endif