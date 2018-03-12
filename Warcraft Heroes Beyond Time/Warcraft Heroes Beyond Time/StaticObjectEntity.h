#ifndef __STATICOBJECTENTITY_H__
#define __STATICOBJECTENTITY_H__

#include "StaticEntity.h"

class StaticObjectEntity : public StaticEntity {
public:
	StaticObjectEntity(fPoint coor, STATIC_OBJECT_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Finish();

private:
	STATIC_OBJECT_TYPE type = STATIC_OBJECT_TYPE::NON_SOBJECT;
};

#endif