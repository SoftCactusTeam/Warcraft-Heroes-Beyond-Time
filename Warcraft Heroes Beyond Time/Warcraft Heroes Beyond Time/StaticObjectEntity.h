#ifndef __STATICOBJECTENTITY_H__
#define __STATICOBJECTENTITY_H__

#include "StaticEntity.h"

class StaticObjectEntity : public StaticEntity {
public:
	StaticObjectEntity(fPoint coor, SDL_Texture* texture);

	bool Start();
	bool Finish();

private:
};

#endif