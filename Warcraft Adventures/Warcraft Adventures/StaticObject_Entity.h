#ifndef __StaticObjectEntity_H__
#define __StaticObjectEntity_H__

#include "Entity.h"

class StaticObject_Entity : public StaticEntity {
public:
	StaticObject_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	void Start();
	void Finish();
	void Draw();

	void Interaction();
};

#endif